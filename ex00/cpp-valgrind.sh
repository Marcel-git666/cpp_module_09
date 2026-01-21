#!/bin/bash

# cpp-valgrind.sh
# Simple script to run C++ exercises with Valgrind in Podman

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Function to start Podman
start_podman() {
    echo -e "${BLUE}Starting Podman machine...${NC}"
    if ! podman machine list | grep -q "Currently running"; then
        podman machine start
        echo -e "${GREEN}Podman started.${NC}"
    else
        echo -e "${YELLOW}Podman already running.${NC}"
    fi
}

# Function to stop Podman
stop_podman() {
    echo -e "${BLUE}Stopping Podman machine...${NC}"
    podman machine stop
    echo -e "${GREEN}Podman stopped.${NC}"
}

# Function to run program normally
# Accepts arguments as $1
run_normal() {
    local PROG_ARGS="$1"
    echo -e "${BLUE}Running program normally with args: ${YELLOW}$PROG_ARGS${NC}"
    podman run --rm -it \
        -v "$(pwd):/workspace" \
        -w /workspace \
        gcc:latest \
        bash -c "make re && ./$EXEC_NAME $PROG_ARGS"
}

# Function to run with Valgrind
# Accepts arguments as $1
run_valgrind() {
    local PROG_ARGS="$1"
    echo -e "${BLUE}Running with Valgrind with args: ${YELLOW}$PROG_ARGS${NC}"
    podman run --rm -it \
        -v "$(pwd):/workspace" \
        -w /workspace \
        gcc:latest \
        bash -c "apt-get update -qq && apt-get install -y -qq valgrind && make re && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$EXEC_NAME $PROG_ARGS"
}

# Show usage
show_usage() {
    echo -e "${BLUE}Usage:${NC} $0 [COMMAND] [EXECUTABLE_NAME] [ARGS...]"
    echo
    echo "Commands:"
    echo "  run [name] [args]      - Run executable normally"
    echo "  valgrind [name] [args] - Run with Valgrind"
    echo "  start                  - Start Podman machine"
    echo "  stop                   - Stop Podman machine"
    echo "  status                 - Check Podman status"
    echo "  help                   - Show this help"
    echo
    echo "Examples:"
    echo "  $0 run input.txt           (Uses NAME from Makefile)"
    echo "  $0 run btc input.txt       (Explicit executable name)"
    echo "  $0 valgrind input.txt"
}

# Helper to get default name from Makefile
get_makefile_name() {
    if [ -f "Makefile" ]; then
        grep -E "^NAME\s*=" Makefile | head -1 | cut -d'=' -f2 | tr -d ' '
    fi
}

# Check Podman status
check_status() {
    echo -e "${BLUE}Podman machine status:${NC}"
    podman machine list
}

# Logic to resolve EXEC_NAME and ARGS
resolve_exec_and_args() {
    # $1 is the first argument after command (e.g., 'btc' or 'input.txt')
    # Remaining args are in $@ (excluding $1 if we shift)

    DEFAULT_NAME=$(get_makefile_name)

    if [ -z "$DEFAULT_NAME" ]; then
        # No Makefile, first arg MUST be exec name
        if [ -z "$1" ]; then
            EXEC_NAME="a.out"
        else
            EXEC_NAME="$1"
            shift
        fi
    else
        # Makefile exists
        if [ "$1" == "$DEFAULT_NAME" ]; then
            # User explicitly typed the name (e.g. ./script run btc input.txt)
            EXEC_NAME="$1"
            shift
        else
            # User probably typed just args (e.g. ./script run input.txt)
            EXEC_NAME="$DEFAULT_NAME"
        fi
    fi

    # All remaining arguments are program arguments
    PROGRAM_ARGS="$@"
}

# Main
case "$1" in
    start)
        start_podman
        ;;
    stop)
        stop_podman
        ;;
    status)
        check_status
        ;;
    run)
        shift # Remove 'run'
        resolve_exec_and_args "$@"
        echo -e "${BLUE}Executable: $EXEC_NAME${NC}"
        start_podman
        run_normal "$PROGRAM_ARGS"
        ;;
    valgrind)
        shift # Remove 'valgrind'
        resolve_exec_and_args "$@"
        echo -e "${BLUE}Executable: $EXEC_NAME${NC}"
        start_podman
        run_valgrind "$PROGRAM_ARGS"
        ;;
    help|--help|-h)
        show_usage
        ;;
    *)
        # Interactive mode
        DEFAULT_NAME=$(get_makefile_name)
        [ -z "$DEFAULT_NAME" ] && DEFAULT_NAME="a.out"

        echo -e "${BLUE}Executable detected: $DEFAULT_NAME${NC}"
        EXEC_NAME=$DEFAULT_NAME

        echo -e "${BLUE}How would you like to run?${NC}"
        echo "1) Normal mode"
        echo "2) Valgrind"
        echo "3) Just start Podman"
        echo "4) Stop Podman"
        read -p "Enter choice (1-4): " choice

        case $choice in
            1)
                read -p "Enter arguments (optional): " PROGRAM_ARGS
                start_podman
                run_normal "$PROGRAM_ARGS"
                ;;
            2)
                read -p "Enter arguments (optional): " PROGRAM_ARGS
                start_podman
                run_valgrind "$PROGRAM_ARGS"
                ;;
            3)
                start_podman
                ;;
            4)
                stop_podman
                ;;
            *)
                echo -e "${RED}Invalid choice.${NC}"
                show_usage
                ;;
        esac
        ;;
esac

#!/usr/bin/env bash
set -e

# Helper to run the project via docker-compose on macOS with XQuartz
# Usage: ./scripts/run_with_xquartz.sh

echo "Make sure XQuartz is running and 'Allow connections from network clients' is enabled."
# Allow local connections
xhost + 127.0.0.1 || true

# Use host.docker.internal on macOS to reach the host X server
export DISPLAY=host.docker.internal:0
export DOCKER_DISPLAY=host.docker.internal:0

# Build and run
docker compose build
docker compose up

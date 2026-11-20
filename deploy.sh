
#!/usr/bin/env bash
set -euo pipefail

# Configuration
REMOTE="pi-server"
REMOTE_DIR="/home/kronborg/castleCraft"
LOCAL_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "==> Syncing CastleCraft..."

rsync -avz --progress \
    --exclude='build/' \
    --exclude='build-arm64/' \
    --exclude='.git/' \
    "$LOCAL_DIR/" \
    "$REMOTE:$REMOTE_DIR/"

echo "==> Building CastleCraft..."

ssh -t "$REMOTE" "
    set -e
    cd '$REMOTE_DIR'

    cmake -S . -B build
    cmake --build build -j2

    echo '==> Restarting Minecraft server...'
    sudo systemctl restart minecraft

    echo '==> Checking server status...'
    sudo systemctl status minecraft --no-pager
"

echo "==> Deployment complete!"


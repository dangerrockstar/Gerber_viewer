# Gerber_viewer
Gerber274x viewer
# Gerber_viewer
Minimal Gerber 274x viewer (Qt + OpenCV)

Usage
 - Open a Gerber file: use the toolbar Open Gerber action.
 - Drag & drop: drop a .gbr/.gtl/.gts/.ger file or a folder containing Gerber files onto the app window.
 - Zip archives: drop a .zip containing Gerber files; the app will extract and open the first Gerber found.

Quick (development)
 - Build with qmake/make or use the included Docker setup for a dev container.

Docker
 - See `Dockerfile` and `docker-compose.yml` for a development container that mounts the source for live editing.

Notes
 - The app defaults file dialogs to the user's Downloads folder.
 - Zip extraction uses the system `unzip` command; ensure it's available in your environment.
- If you prefer an interactive shell instead of auto-running the binary, run `docker compose run --rm app bash`.
- Build artifacts are kept in the named volume `gerber_viewer_build_data` and source is mounted from the host so edits are immediate.

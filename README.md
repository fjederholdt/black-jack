# Black-Jack

Black-Jack is a Qt6-based C++ desktop implementation of the classic Blackjack card game. The application provides a graphical Windows interface for playing Blackjack with support for core actions such as Hit, Stand, Double Down, Split, Surrender, and betting management.

## Features

- Qt6 GUI with card image display and dealer/player layout
- Blackjack gameplay rules including player hand splitting and double down
- Dealer behavior and bust handling
- Betting system with current balance tracking
- Deck management and shuffle logic
- Unit tests using GoogleTest for hand and deck behavior

## Project Structure

- `Black-Jack/` — Qt application source code and project files
- `Black-Jack/header/` — game object and controller headers
- `Black-Jack/src/` — implementation files for game logic and UI
- `Black-Jack/unit_tests/` — GoogleTest unit tests
- `spillekort/` — card image assets loaded at runtime

## Build Requirements

- CMake (3.10 or newer)
- Qt6 with Widgets module
- A C++17-compatible compiler

## Download Artifacts

The project publishes build artifacts in GitHub Actions using the workflow at `.github/workflows/build.yml`. The artifact name is `Black-Jack-Windows`.

To download the latest build artifact, open the repository Actions page, select the `Build Qt App (Windows)` workflow, click the Artifacts number and scroll down to download the `Black-Jack-Windows` artifact.

Unpack it and run `Black-Jack.exe` to start the application.

## Notes

The application expects to find a `spillekort` directory containing card image assets, including `Backside.png`. If the folder or assets are missing, the app displays an error message on startup.

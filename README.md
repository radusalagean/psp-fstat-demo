# psp-fstat-demo

Tested on Ubuntu 24.04

## PSP Dev Env setup (Ubuntu)
- Install the required dependencies:
    ```bash
    bash <(curl -s https://raw.githubusercontent.com/pspdev/pspdev/v20241001/prepare.sh)
    ```
    ```bash
    sudo apt install make cmake genisoimage
    ```
- Get the [`v20241001`](https://github.com/pspdev/pspdev/releases/tag/v20241001) `pspdev-ubuntu-latest-x86_64.tar.gz` archive from the releases page and extract it
- Move the `pspdev` directory to `~/pspdev`
- Add in `~/.bashrc`:
    ```bash
    export PSPDEV=~/pspdev
    export PATH=$PATH:$PSPDEV/bin
    ```
- Run `source ~/.bashrc`

## Compile the example source

- `cd` to the project root directory
- `chmod +x compile.sh`
- `./compile.sh`
#!/bin/sh

declare -a msys2_pacman_dep=("base-devel")
declare -a msys2_pacman_mingw_dep=(
  "glfw"
  "glew"
  "glm"
)


confirmation()
{
  while true; do
    read -p "$1 [y/n] " yn
    case $yn in
      [Yy]* ) $2; return;;
      [Nn]* ) return;;
      * ) echo "Please answer yes or no.";;
    esac
  done
}

install_packages()
{
  echo "Installing packages"
  for val in ${msys2_pacman_mingw_dep[@]}; do
    pacman -S --noconfirm mingw-w64-x86_64-$val
  done
}

prompt_install()
{
  case "$(uname -a)" in
    MINGW64*|*Msys )
      confirmation "Install dependency packages and prepare environment for development?" install_packages;
      ;;
    * )
      echo "Error: Unsupported platform"
      return 
      ;;
  esac

}

case "$1" in
  "run" ) (cd build && exec ./clearsdf);;
  "build" ) cmake --build ./build --config Debug;;
  "buildrun" ) cmake --build ./build --config Debug && (cd build && exec ./clearsdf);;
  "devprep" ) prompt_install;;
esac

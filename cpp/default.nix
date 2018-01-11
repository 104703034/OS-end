let
  pkgs = import <nixpkgs> {};
  stdenv = pkgs.stdenv;
in rec {
  bankersEnv = stdenv.mkDerivation rec {
    name = "bankers-env" ;
    version = "0.1" ;
    src = ./src ;
    buildInputs = (with pkgs; [
                    cmake gnumake
                    pkgconfig
                    x11
                    glfw
                  ]) ++
                  (with pkgs.xorg; [
                    libxcb libpthreadstubs
                    libXdmcp libXrandr libXinerama
                    libXcursor
                  ]);
  } ;
}


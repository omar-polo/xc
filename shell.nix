{ pkgs ? import <nixpkgs> {} }:
with pkgs;
mkShell {
 buildInputs = [
   autoconf automake libstrophe
 ];
 }

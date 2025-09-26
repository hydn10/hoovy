{ pkgs ? import <nixpkgs> {} }:


let
  package = pkgs.callPackage ./. {};
in
  pkgs.mkShell.override { stdenv = package.stdenv; } {
    inputsFrom = [ package ];
    nativeBuildInputs = [
      pkgs.clang-tools
      pkgs.gdb
    ];
  }

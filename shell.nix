{ pkgs ? import <nixpkgs> {} }:


let
  package = pkgs.callPackage ./. {};

  clang-tools = pkgs.clang-tools.override {
    llvmPackages = pkgs.llvmPackages_14;
  };
in
  pkgs.mkShell.override { stdenv = package.stdenv; } {
    inputsFrom = [ package ];
    nativeBuildInputs = [
      clang-tools
      pkgs.gdb
    ];
  }

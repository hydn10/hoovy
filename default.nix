{ lib, stdenv, htracer, ffmpeg, cmake, pkg-config }:

let
  vcpkgFile = builtins.fromJSON (builtins.readFile ./vcpkg.json);

  pname = vcpkgFile.name;
  version = vcpkgFile.version;
in
  stdenv.mkDerivation
  {
    inherit pname;

    name = "${pname}-${version}";
    inherit version;

    src = lib.cleanSource ./.;

    buildInputs = [ htracer ffmpeg ];
    nativeBuildInputs = [ cmake pkg-config ];
  }

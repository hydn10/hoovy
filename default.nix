{ lib, stdenv, htracer, ffmpeg, cmake, pkg-config }:

let
  pname = "hoovy";
  version = "0.1.0";
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

{
  description = "A simple C++ video rendering from raytraced scenes.";

  inputs = {
    nixpkgs.url = "nixpkgs";
    htracer.url = "github:hydn10/htracer";
  };

  outputs = { self, nixpkgs, htracer }:
    let
      pkgs-lin64 = import nixpkgs {
        system = "x86_64-linux";
        overlays = [ htracer.overlays.default ];
      };

      hoovyDrv-lin64 = pkgs-lin64.callPackage ./default.nix {};
    in
    {
      apps.x86_64-linux.hoovy = 
      {
        type = "app";
        program = "${self.packages.x86_64-linux.hoovy}/bin/hoovy";
      };

      apps.x86_64-linux.default = self.apps.x86_64-linux.hoovy;

      overlays.default = final: prev: { hoovy = self.packages.x86_64-linux.hoovy; };

      packages.x86_64-linux.hoovy = hoovyDrv-lin64;
      packages.x86_64-linux.default = self.packages.x86_64-linux.hoovy;

      devShells.x86_64-linux.default = import ./shell.nix { pkgs = pkgs-lin64; };
    };
}

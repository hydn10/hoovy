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

      packageDrv-lin64 = pkgs-lin64.callPackage ./default.nix {};

      pkgName = packageDrv-lin64.pname;
    in
    {
      apps.x86_64-linux.${pkgName} =
      {
        type = "app";
        program = "${self.packages.x86_64-linux.${pkgName}}/bin/${pkgName}";
      };

      apps.x86_64-linux.default = self.apps.x86_64-linux.${pkgName};

      overlays.default = final: prev: { ${pkgName} = self.packages.x86_64-linux.${pkgName}; };

      packages.x86_64-linux.${pkgName} = packageDrv-lin64;
      packages.x86_64-linux.default = self.packages.x86_64-linux.${pkgName};

      devShells.x86_64-linux.default = import ./shell.nix { pkgs = pkgs-lin64; };
    };
}

{
  description = "A simple C++ video rendering from raytraced scenes.";

  inputs = {
    nixpkgs.url = "nixpkgs";
    hydn-pkgs.url = "github:hydn10/htracer";
  };

  outputs = { self, nixpkgs, hydn-pkgs }:
    let
      pkgs-lin64 = import nixpkgs {
        system = "x86_64-linux";
        overlays = [ hydn-pkgs.overlay ];
      };

      hoovyDrv-lin64 = pkgs-lin64.callPackage ./default.nix {};
    in
    {
      apps.x86_64-linux.ray = 
      {
        type = "app";
        program = "${self.x86_64-linux.packages.hoovy}/bin/ray";
      };

      # From nix >= 2.7 this should be 'packages.<system>.default'
      defaultApp.x86_64-linux = self.apps.x86_64-linux.hoovy;

      overlay = final: prev: { hoovy = self.packages.x86_64-linux.hoovy; };

      packages.x86_64-linux.hoovy = hoovyDrv-lin64;

      # From nix >= 2.7 this should be 'packages.<system>.default'
      defaultPackage.x86_64-linux = self.packages.x86_64-linux.hoovy;

      # From nix >= 2.7, this should be 'devShells.<system>.default'
      devShell.x86_64-linux = import ./shell.nix { pkgs = pkgs-lin64; };
    };
}

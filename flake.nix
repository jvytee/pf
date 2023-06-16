{
  inputs = {
    nixpkgs.url = "nixpkgs/nixos-23.05";
  };

  outputs = { self, nixpkgs }: 
  let
    systems = ["x86_64-linux"];
    genAttrs = nixpkgs.lib.genAttrs systems;
  in
  {
    devShells = genAttrs (system:
    with import nixpkgs { inherit system; };
    {
      default = clangStdenv.mkDerivation {
        name = "pf-shell";

        nativeBuildInputs = [
          clang-tools
          meson
          ninja
          pkg-config
        ];

        buildInputs = [
          check
          libpng
        ];
      };

      python = mkShell {
        buildInputs = [
          python39
          python39Packages.pillow
          nodepackages.pyright
        ];
      };
    });

    packages = genAttrs (system:
    with import nixpkgs { inherit system; };
    {
      pf = clangStdenv.mkDerivation {
        pname = "pf";
        version = "0.1.0";
        src = self;

        nativeBuildInputs = [
          meson
          ninja
          pkg-config
        ];
        
        buildInputs = [
          check
          libpng
        ];

        mesonBuildType= "release";
      };
    });
  };
}

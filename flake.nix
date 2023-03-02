{
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
          check
          clang-tools
          libpng
        ];

        buildInputs = [
          meson
          ninja
          pkg-config
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
  };
}

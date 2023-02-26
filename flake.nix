{
  outputs = { self, nixpkgs }: 
  let
    system = "x86_64-linux";
  in
  with import nixpkgs { inherit system; };
  {
    devShells.${system}.default = clangStdenv.mkDerivation {
      name = "pf-shell";

      nativeBuildInputs = [
        clang-tools
        libpng
      ];

      buildInputs = [
        clang-tools
        meson
        ninja
        pkg-config
      ];
    };
  };
}

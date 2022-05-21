with import <nixpkgs> {};

mkShell {
  buildInputs = [
    python310
    nodePackages.pyright
  ];
}

with import <nixpkgs> {};

mkShell {
  buildInputs = [
    python39
    python39Packages.pillow
    nodePackages.pyright
  ];
}

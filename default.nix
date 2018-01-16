with import <nixpkgs> {}; {
  mlEnv = stdenv.mkDerivation {
    name = "mlEnv";
    buildInputs = [
      cmake
      gcc
      gtest
    ];
  };
}

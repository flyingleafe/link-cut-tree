with import <nixpkgs> {}; {
  mlEnv = stdenv.mkDerivation {
    name = "mlEnv";
    buildInputs = [
      cmake
      gcc
      gtest
      python35
      python35Packages.numpy
      python35Packages.setuptools
    ];
  };
}

with import <nixpkgs> {}; {
  mlEnv = stdenv.mkDerivation {
    name = "mlEnv";
    buildInputs = [
      cmake
      gcc
      gtest
      python36
      python36Packages.numpy
      python36Packages.setuptools
    ];
  };
}

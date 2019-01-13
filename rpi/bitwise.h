char getBit(int n, char a){
  return (a >> n) & 1;
}

void setBit(int n, char a){
  a |= 1 << n;
}

void unsetBit(int n, char a){
  a &= ~(1 << n);
}

void toggleBit(int n, char a){
  a ^= 1 << n;
}

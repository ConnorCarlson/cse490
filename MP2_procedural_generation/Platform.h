class Platform{

  private:
    byte size;
    byte x;
    
  public:
    // Constructor
    Platform() {
      x = 0;
      size = 0;
    };


    void updateX(byte newX) {
      x = newX;
    }

    void updateSize(byte newSize) {
      size = newSize;
    }

    int getSize() {
      return size;
    }
    int getX() {
      return x;
    }
};

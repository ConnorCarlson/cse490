class Platform{

  private:
    int size;
    int x;
    
  public:
    // Constructor
    Platform() {
      x = 0;
      size = 0;
    };


    void updateX(int newX) {
      x = newX;
    }

    void updateSize(int newSize) {
      size = newSize;
    }

    int getSize() {
      return size;
    }
    int getX() {
      return x;
    }
};

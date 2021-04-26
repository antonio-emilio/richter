#ifndef PCA8574_H
#define PCA8574_H

#define _PCA8574_SlaveAddress    0x20


class PCA8574{
public:
  PCA8574();
  int pca8574_get_data(unsigned char &data);
  int pca8574_put_data(unsigned char data);
};

#endif // PCA8574_H

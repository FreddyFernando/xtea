//============================================================================
// Name        : xtea.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <stdint.h>
#include <cstring>
using namespace std;


unsigned int key[4]={0xFDA5,0xD54E,0xFC00,0xB55A}; // encryption key

#define BLOCK_SIZE 8
void xtea_encipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]) {
    unsigned int i;
    uint32_t v0=v[0], v1=v[1], sum=0, delta=0x9E3779B9;
    for (i=0; i < num_rounds; i++) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
    }
    v[0]=v0; v[1]=v1;
}

void xtea_decipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]) {
    unsigned int i;
    uint32_t v0=v[0], v1=v[1], delta=0x9E3779B9, sum=delta*num_rounds;
    for (i=0; i < num_rounds; i++) {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
        sum -= delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
    }
    v[0]=v0; v[1]=v1;
}

void FileCrypt(string filename,bool encrypt)
{
  fstream file(filename.c_str(),ios::in | ios::out | ios::binary);

  if(!file)
    {
      cout <<"Could not open file";
      return;
    }

  unsigned size;

  file.seekg(0,ios::end);
  size=file.tellg();
  file.seekg(ios::beg);

  file.clear();

  unsigned pos;

  int n_blocks=size/BLOCK_SIZE;
  if(size%BLOCK_SIZE!=0)
      ++n_blocks;

  for(int i=0;i<n_blocks;i++)
    {
      unsigned char data[BLOCK_SIZE];
      pos=file.tellg();

      file.read((char*)data,BLOCK_SIZE); // read data block

      if(encrypt)
          xtea_encipher(32,(uint32_t*)data,key);
      else
          xtea_decipher(32,(uint32_t*)data,key);

      file.seekp(pos);
      file.write((char*)data,BLOCK_SIZE);

      memset(data,0,BLOCK_SIZE);
    }
  file.close();
}

int main()
{
	bool EncDec = true;
	cout << "Do you want to Encrypt (1) or Decrypt (0)?";
	cin >> EncDec;
	if(EncDec)
		cout << "Encrypting";
	else
		cout << "Decrypting";
   //FileCrypt("file.txt",true); // encrypt
   FileCrypt("file.txt",EncDec);
}

#include <SPI.h>
#include <SD.h>

#define CONF_CURRENT_MODEL_POS  (0)
#define CONF_CALIBRATION_POS    (1)

#define MODEL_NAME_POS  (0)
#define MODEL_MIXER_POS  (8)
#define MODEL_TRIMS_POS  (MODEL_MIXER_POS + PPM_MAX_CHANNELS * PPM_MAX_CHANNELS * sizeof(int))

class Storage
{
public:
  void init();

  void save_current_model(int model);
  void load_current_model(int* model);

  void save_calibration(int* data);
  void load_calibration(int* data);

  void save_name(int model, char* data);
  void load_name(int model, char* data);
  
  void save_mixer(int model, int* data);
  void load_mixer(int model, int* data);

  void save_trims(int model, int* data);
  void load_trims(int model, int* data);

private:
  void write_int_array(File file, int pos, int* data, int size);

  void read_int_array(File file, int pos, int* data, int size);
};

void Storage::init()
{
  Serial.println("Initializing storage ... ");
  
  if(!SD.begin(4))
  {
    Serial.println("Error initializing SD");
  }
}

void Storage::save_current_model(int model)
{
  File file = SD.open("SPT_CONF", FILE_WRITE);
  if(file)
  {
    file.seek(CONF_CURRENT_MODEL_POS);
    file.write(model);
    file.close();
  }
}

void Storage::load_current_model(int* model)
{
  File file = SD.open("SPT_CONF", FILE_READ);
  if(file && file.available())
  {
    file.seek(CONF_CURRENT_MODEL_POS);
    *model = file.read();
    file.close();
  }
}

void Storage::save_calibration(int* data)
{
  File file = SD.open("SPT_CONF", FILE_WRITE);
  if(file)
  {
    write_int_array(file, CONF_CALIBRATION_POS, data, 4 * 2);
    file.close();
  }
}

void Storage::load_calibration(int* data)
{
  File file = SD.open("SPT_CONF", FILE_READ);
  if(file && file.available())
  {
    read_int_array(file, CONF_CALIBRATION_POS, data, 4 * 2);
    file.close();
  }
}

void Storage::save_name(int model, char* data)
{
  String prefix = "MODEL";
  String filename = String(prefix + model); 
  File file = SD.open(filename, FILE_WRITE);
  if(file)
  {
    file.seek(MODEL_NAME_POS);
    file.write(data, 8);
    file.close();
  }
}

void Storage::load_name(int model, char* data)
{
  String prefix = "MODEL";
  String filename = String(prefix + model); 
  File file = SD.open(filename, FILE_READ);
  if(file && file.available())
  {
    file.seek(MODEL_NAME_POS);
    file.read(data, 8);
    file.close();
  }
  else
  {
    //String modelname = String(prefix + model); 
    filename.toCharArray(data, 9);
  }
}

void Storage::save_mixer(int model, int* data)
{
  String prefix = "MODEL";
  String filename = String(prefix + model); 
  File file = SD.open(filename, FILE_WRITE);
  if(file)
  {
    write_int_array(file, MODEL_MIXER_POS, data, PPM_MAX_CHANNELS * PPM_MAX_CHANNELS);
    file.close();
  }
}

void Storage::load_mixer(int model, int* data)
{
  String prefix = "MODEL";
  String filename = String(prefix + model); 
  File file = SD.open(filename, FILE_READ);
  if(file && file.available())
  {
    read_int_array(file, MODEL_MIXER_POS, data, PPM_MAX_CHANNELS * PPM_MAX_CHANNELS);
    file.close();
  }
}

void Storage::save_trims(int model, int* data)
{
  String prefix = "MODEL";
  String filename = String(prefix + model);  
  File file = SD.open(filename, FILE_WRITE);
  if(file)
  {
    write_int_array(file, MODEL_TRIMS_POS, data, PPM_MAX_CHANNELS);
    file.close();
  }
}

void Storage::load_trims(int model, int* data)
{
  String prefix = "MODEL";
  String filename = String(prefix + model); 
  File file = SD.open(filename, FILE_READ);
  if(file && file.available())
  {
    read_int_array(file, MODEL_TRIMS_POS, data, PPM_MAX_CHANNELS);
    file.close();
  }
}

void Storage::write_int_array(File file, int pos, int* data, int size)
{
  file.seek(pos);
  file.write((char*) data, size * sizeof(int));
}

void Storage::read_int_array(File file, int pos, int* data, int size)
{
  file.seek(pos);
  file.read((char*) data, size * sizeof(int));
}



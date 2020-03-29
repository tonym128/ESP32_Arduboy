// Ard Drivin Converter
// This tool will process all files from the 'data' subfolder, and output the result
// auto-generated .h files in the parent folder 'pics'

// Conversion mode 0 means normal Arduboy 1-bit per pixel: 0=black, 1=white, compatible with drawBitmap and font
// Conversion mode 1 means special 2-bit per pixel: 00=black, 01=transparent, 10=gray, 11=white, use drawGrayBitmap
// Conversion mode 2 means masked+data, total 2-bit per pixel, stored as 1-byte mask, 1-byte data, etc.: 0=black, 1=white, use drawMaskedBitmap
int globalConversionMode = 2;
String outputFolder = "../pics";

PImage img;  // Declare a variable of type PImage
PrintWriter output;

class Hotspot {
  int x;
  int y;
  
  Hotspot(int inX, int inY) {
    x = inX;
    y = inY;
  }
};

HashMap<String, Hotspot> hotspots = new HashMap<String, Hotspot>();

void CalculatePerspective() {
  println("Done calculating. Comment the line CalculatePerspective(); in setup() to remove this call.");
  String result = "const uint8_t PROGMEM yLookup[256] = { ";
  // ylookup array maps a y value from 0..255 to a perspective road projected value of y range 20..63
  for(int i=0; i<256; i++) {
    float divisor = 31.32 - i / 10.0;
    byte y = (byte) (20 + i / divisor);
    result += y + ", ";
  }
  result += "};";
  println(result);
  exit();
}

void setup() {
  //CalculatePerspective();
  
  size(512,256);
  String lines[] = loadStrings("hotspot.txt");
  for (int i = 0; i < lines.length; i++) {
    String[] tokens = split(trim(lines[i]), ' ');
    if (tokens.length == 0) continue;
    
    if (tokens.length != 3) {
      println("Erroneous hotspot in 'hotspot.txt' on line "+i+": "+lines[i]+" ("+tokens.length+" tokens): expected:  <filename> x y");
      exit();
      return;
    }
    hotspots.put(tokens[0], new Hotspot(int(tokens[1]), int(tokens[2])));
  }
  println("Parsed " + hotspots.size() + " hotspot definition.");
  
  File dir = new File(dataPath(""));
  String[] children = dir.list();
  if (children == null) {
      println("Can't list file in data folder");
      exit();
      return;
  }
  for(int i=0 ;i < children.length; i++) {
    String fileName = children[i];
    if (!fileName.contains(".png")) continue;
    
    println("Found file: "+fileName+": Processing it...");
    String[] fileSplits = fileName.split("\\.");
    convertImage(fileSplits[0]);
  }
}

void convertImage(String baseName) {
  int conversionMode = globalConversionMode;
  // Special case for 'font': it always uses conversionmode 0 (i.e.: black&white, no mask, no hotspot)
  if (baseName.equals("font")) {
     println("Font detected, using conversion mode 0");
    conversionMode = 0;
  }
  
  Hotspot hotspot = hotspots.get(baseName);
  if (hotspot == null) {
    if (conversionMode != 0) {
      println("Warning: no hotspot defined for bitmap "+baseName+". Assuming 0, 0.");
    }
    hotspot = new Hotspot(0, 0);
  }
  
  String inputName = baseName + ".png";
  String outputName = outputFolder + "/" + baseName + ".h";
  
  // Make a new instance of a PImage by loading an image file
  img = loadImage(inputName);
  
  // Before we deal with pixels
  img.loadPixels();
  // Loop through every pixel, accessed vertically like the Arduboy oled display is:
  // if conversionMode==0, then:
  // i.e.: 1-bit per pixel, 8 pixels are stored vertically in one byte
  // byte[0]=pixels(0,0 to 0,7)  byte[1]=pixels(1,0 to 1,7), etc..

   // if conversionMode==1, then:
  // i.e.: 2-bit per pixel, 4 pixels are stored vertically in one byte
  // byte[0]=pixels(0,0 to 0,3)  byte[1]=pixels(1,0 to 1,3), etc..
  
  int pageHeight = 8;
  if (conversionMode == 1) {
    pageHeight = 4;
  }

  int w = img.width;
  int h = img.height;
  println("Input image is: "+inputName+" size: "+w+"x"+h+". Outputing result to: "+outputName+" using conversion mode "+conversionMode);
  
/*  if ((h & (pageHeight-1)) != 0) {
    println("Error: In conversionMode "+conversionMode+", the height of the bitmap must be divisible by "+pageHeight+" !");
    exit();
    return;
  }*/
  
  output = createWriter(outputName);
  switch (conversionMode) {
    case 0:
      output.println("// Bitmap definition for arduboy '" + inputName + "' in 1-bit (black/white)");
      break;
    case 1:
      output.println("// Bitmap definition for arduboy '" + inputName + "' in 2-bit (transparent/black/gray/white)");
      break;
    case 2:
      output.println("// Bitmap definition for arduboy '" + inputName + "' in 1-bit mask + 1-bit data in two bytes");
      break;
  }
  output.println("#include <avr/pgmspace.h>");
  output.println();
  output.println("#ifndef " + baseName + "_H");
  output.println("#define " + baseName + "_H");
  output.println();

  colorMode(RGB, 255);
  
  int paddedHeight = ((h + 7) / 8) * 8;
  
  int numberOfByteToOutput = w * paddedHeight / pageHeight;
  
  output.println("static const unsigned char " + baseName + "[] PROGMEM = {");  
  if (conversionMode == 0) {
    for(int p=0; p<h; p+=pageHeight) {
      output.print("\t");
      
      for(int x=0; x<w; x++) {
        int b = 0;
        for (int y = p; y < p+pageHeight; y++) {
          int index = x + y * w;
          color c = img.pixels[index];
          float gray = brightness(c);
          b >>= 1;
          float red = red(c);
          float green = green(c);
          float blue = blue(c);
          // If pixel is transparent (255,0,255), ignore
          if (red == 255 && green == 0 && blue == 255) {
          } else if (gray >= 85) {
            b |= (1 << 7);
            img.pixels[index] = color(255, 255, 255);
          }
        }
        output.print("0x"+hex(b, 2));
        numberOfByteToOutput--;
        if (numberOfByteToOutput > 0) {
          output.print(",");
        }
      }
      output.println();
    }
  } else if (conversionMode == 2) {
    // Mask + Data in two consecutive byte
    // Almost emit bitmap size (width, height) as the two first bytes, and hotspot (x,y) as another two bytes
    output.println("\t// Size of bitmap: " + w + " x " + h + ". Hotspot at :"+hotspot.x + ", "+hotspot.y);
    output.println("\t" + w + "," + h + "," + hotspot.x + "," + hotspot.y + ",");
    
    for(int p=0; p<paddedHeight; p+=pageHeight) {
      output.print("\t");
      
      for(int x=0; x<w; x++) {
        int b = 0;
        int maskb = 0;
        for (int y = p; y < p+pageHeight; y++) {
          // If y is outside the real image, due to padding to have a multiple of 8, treat padding as transparent
          color c = color(255, 0, 255);
          if (y < h) {
            c = img.pixels[x + y * w];
          }
          
          float gray = brightness(c);
          b >>= 1;
          maskb >>= 1;
          float red = red(c);
          float green = green(c);
          float blue = blue(c);
          float alpha = alpha(c);
          // If pixel is transparent or pure magenta (255,0,255), treat as transparent
          if ((red == 255 && green == 0 && blue == 255) || (alpha < 128)) {
            //img.pixels[index] = color(0, 127, 0);
          } else {
             // if it not transparent nor white, emit 1 (i.e.: only need to mask black portion)
             maskb |= (1 << 7);
             if (gray >= 85) {
               b |= (1 << 7);
               //img.pixels[index] = color(255, 255, 255);
             }
          }
        }
        output.print("0x"+hex(maskb, 2) + "," + "0x"+hex(b, 2));
        numberOfByteToOutput--;
        if (numberOfByteToOutput > 0) {
          output.print(",");
        }
      }
      output.println();
    }
  } else if (conversionMode == 1) { // Store 2-bit bitmap vertically
    for(int x=0; x<w; x++) {
      output.print("\t");
      for(int p=0; p<h; p+=pageHeight) {
        int b = 0;
        for (int y = p; y < p+pageHeight; y++) {
          int index = x + y * w;
          color c = img.pixels[index];
          float gray = brightness(c);
          b >>= 2;
          float red = red(c);
          float green = green(c);
          float blue = blue(c);
          // If pixel is transparent (255,0,255), emit 01
          if (red == 255 && green == 0 && blue == 255) {
            gray = 70;
            b |= (1 << 6);
          } else if (gray < 85) { // Else if it is black, emit 00
          } else if (gray > 170) { // Else if it is white, emit 11
            b |= (3 << 6);
          } else { // Else, it is gray: emit 10
            b |= (2 << 6);
          }
          img.pixels[index] = color(gray, gray, gray);
        }
        output.print("0x"+hex(b, 2));
        numberOfByteToOutput--;
        if (numberOfByteToOutput > 0) {
          output.print(",");
        }
      }
      output.println();
    }
  }
  output.println("};");
  
  output.println();
  output.println("#endif");
  
  output.flush(); // Writes the remaining data to the file
  output.close(); // Finishes the file
  
  // When we are finished dealing with pixels
  img.updatePixels(); 
}

void draw() {
  background(0);
  // Draw the image to the screen at coordinate (0,0)
  scale(4);
  if (img != null) {
    image(img,0,0);
  }
}
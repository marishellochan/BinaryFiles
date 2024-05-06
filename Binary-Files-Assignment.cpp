//     Item 2: created GIF successfully? Yes/No // yes
//     Item 3: created GIF successfully? Yes/No // yes
//     Item 4: created GIF successfully? Yes/No // yes
//     Item 5: created GIF successfully? Yes/No // yes
//     Item 6: created GIF successfully? Yes/No // yes
//     Item 13: created animated GIF of Scenery successfully? Yes/No // yes
//     Item 16: created the animated GIF of random images successfully? Yes/No  // yes
//    Item 17: Web page launches successfully from program? Yes/No // yes

//    Any other comments?

#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <windows.h>
using namespace std;

struct Image {         // stores the bytes of the image
    unsigned char descriptor [10];
    unsigned char colourTable [768];
    unsigned char data [10000000];
    int sizeData;
};


struct AnimFrame {    // stores each image/frame of the animation and the display time
    Image * image;
    int duration;
};

struct Animation {    // stores all the frames of the animation
    AnimFrame frames[20];
    int numFrames;
};


Image * readImage (char filename[]); // creates an image struct from the GIF file
void saveImage (Image * image, char filename[]); // creates a GIF file from an image struct
Image * copyImage (Image * image); // creates a copy of the image struct
Image * applyEffectImage (Image * image, string effect); // creates a new image struct and then applies effect to it

Animation * initAnimation (); // creates a new animation with no frames
void addFrame (Animation * animation, Image * image, int duration); // creates a frame and adds it to the end of the animation
void saveAnimation (Animation * animation, char filename[]); // creates a GIF file from the animation


Image * readImage (char filename[]) {
    Image * image;
    unsigned char byte;
    ifstream input;
    
    input.open(filename, ios::binary | ios::in);
    
    if(!input){
    	cout << "Error in opening image file. Aborting .." <<endl;
    	exit(1);
	}
	
    image = new Image;
    
    // ignore first 13 bytes in image file
    long filelocation = input.tellg();
    
    input.seekg(filelocation + 13);
    
    // read 10 bytes from image file to descriptor field
		int i;
	   for( i=0; i<10; i++){
	 input.read((char*) &byte, sizeof(byte));
	 image->descriptor[i]=byte;
 	 //cout << sizeof(image->descriptor) << endl;
   }
   
    // read 768 bytes from image file to colourTable field
    
   	 int j;
	 for(j=0; j<768; j++){
	 input.read((char*) &byte, sizeof(byte));
	 image->colourTable[j]=byte;
 	 //cout << sizeof(image->descriptor) << endl;
   }

    
    
    // read remaining bytes from image file into data field
    int numberofbytes=0;
    input.read((char*)&byte, sizeof(byte));
    while(!input.eof()){
    	image->data[numberofbytes] = byte;
    	numberofbytes ++;
    	input.read((char*)&byte, sizeof(byte));
    	
	}
	
	numberofbytes --;
    
    image->sizeData = numberofbytes;

    input.close();
    
    /*cout << "Image:" <<endl;
    cout << "Image descriptor :" << image->descriptor <<endl;
    cout << "Image color table :" << image->colourTable << endl;
    cout << " Image Data : " << image->data <<endl;
    cout << "Size of data : " << image->sizeData <<endl << endl <<endl; */
    
    return image;
}




void saveImage (Image * image, char filename[]) {

    unsigned char endByte = 59;
    unsigned char header[6]= {71, 73, 70, 56, 57, 97};
    unsigned char lsd[7]= {88, 2, 144, 1, 112, 0, 0};
    ofstream output;
    
    output.open(filename, ios::binary | ios::out);
    
    if(!output){
    	cout << "Error in opening " << filename << " file. Aborting ... " << endl;
    	exit(1);
	}

    // write bytes from header
    	int header_size = sizeof(header)/ sizeof(header[0]);
    	
    	int i;
    	for(i=0; i<6 ; i++){
    		output.write((char*) &header[i], sizeof(header[i]));
		}
    	
    
    
    
    // write bytes from logical screen desciptor
    int lsd_size = sizeof(lsd)/ sizeof(lsd[0]);
    	
    	int j;
    	for(j=0; j<7 ; j++){
    		output.write((char*) &lsd[j], sizeof(lsd[j]));
		}
    	
    
    	

    
    // write bytes from descriptor field
    	int d_size = sizeof(image->descriptor)/ sizeof(image->descriptor[0]);
    	//output.write((char*) &d_size, sizeof(d_size));
    	
    	int k;
    	for(k=0; k<10 ; k++){
    		output.write((char*) &image->descriptor[k], sizeof(image->descriptor[k]));
		}
    	
    	
    


    // write bytes from colourTable
    int color_size = sizeof(image->colourTable)/ sizeof(image->colourTable[0]);
    	
    	int t;
    	for(t=0; t<768 ; t++){
    		output.write((char*) &image->colourTable[t], sizeof(image->colourTable[t]));
		}
    	
   	 //output.write((char*) image->colourTable, sizeof(image->colourTable));
    	
    
    
    // write sizeData bytes from data field
    	//output.write((char*) &image->sizeData, sizeof(image->sizeData));
    	int h;
    	for(h=0; h<image->sizeData; h++){
    		output.write((char*) &image->data[h], sizeof(image->data[h]));
		}
    	
    //	output.write((char*) image->data, sizeof(image->data));
        
    
    // write terminating byte
    	output.write((char*) &endByte, sizeof(endByte));
    	
    	output.close();

}



Image * copyImage (Image * image) {
    Image * newImage;

    newImage = new Image;
    
    // copy the descriptor
    
    	int i=0;
    	
    	while(i<10){
    		
    		newImage->descriptor[i] = image->descriptor[i];
    		i++;
		}
    
    	
    
    
    // copy the colourTable
    
    	 i=0;
    	
    	while(i<768){
    		
    			newImage->colourTable[i] = image->colourTable[i];
    		i++;
		}
    
    
    
    // copy the data
    	i =0;
    	newImage->sizeData = image->sizeData;
    	
    	while(i<newImage->sizeData){
    		
    			newImage->data[i] = image->data[i];
    			i++;
		}
    
    	
    	
    
    return newImage;
}



void toGrayScale (int * red, int * green, int * blue) {
 
 		int avg = ((*red) + (*green) + (*blue))/3;
 		
 		*red = avg;
 		
 		*green = avg;
 		
 		*blue = avg;
 		

}



void toBlueTint (int * red, int * green, int * blue) {

		int avg = ((*red) + (*green) + (*blue))/3;
	
		*red = avg;
 		
 		*green = avg;
		
	
}



void toRedTint (int * red, int * green, int * blue) {

	int avg = ((*red) + (*green) + (*blue))/3;
 		
 		*green = avg;
 		
 		*blue = avg;
 		
}



void toSepia (int * red, int * green, int * blue) {

	int newRed = ((*red)*0.393)+ ((*green)* 0.769)+ ((*blue)*0.189);
	int newGreen = ((*red)*0.349)+ ((*green)* 0.686)+ ((*blue)*0.168);
	int newBlue = ((*red)*0.272)+ ((*green)* 0.534)+ ((*blue)*0.131);
	
	if (newRed > 255){
		newRed = 255;
	}
	
	if(newGreen > 255){
		newGreen = 255;
	}
	
	if(newBlue > 255){
		newBlue = 255;
	}
	
	*red = newRed;
	*green = newGreen;
	*blue = newGreen;
}



void brighten (int * red, int * green, int * blue, int percent) {

	*red = ((*red)*(percent/100)) + (*red);
	*green = ((*green)*(percent/100)) + (*green);
	*blue = ((*blue)*(percent/100)) + (*blue);
}



Image * applyEffectImage (Image * image, string effect) {
    Image * newImage;

    // copy image to newImage and apply effect to bytes in colourTable of newImage
    
    newImage = copyImage(image);
    
    int num_bytes=0;
    
    int * red = new int ;
    int * green = new int ;
    int * blue = new int;

//	int red;
//	int green;
//	int blue;
    
    
    if(effect == "grayScale"){
    	
    	while(num_bytes < 768){
    		
    		*red = newImage->colourTable[num_bytes];
    		
    		*green = newImage-> colourTable[num_bytes+1];

    		*blue = newImage-> colourTable[num_bytes+2];
    		
    		toGrayScale(red, green, blue);
    		
    		newImage->colourTable[num_bytes] = *red;
    		newImage->colourTable[num_bytes+1] = *green;
    		newImage->colourTable[num_bytes+2] = *blue;
    		
    		num_bytes= num_bytes + 3;
    	
			}
    	
    	
	}
	
	if(effect == "blueTint"){
    	
    	while(num_bytes < 768){
    		*red = newImage->colourTable[num_bytes];
    		
    		*green = newImage-> colourTable[num_bytes+1];

    		*blue = newImage-> colourTable[num_bytes+2];
    		
    		toBlueTint(red, green, blue);
    		
    		newImage->colourTable[num_bytes] = *red;
    		newImage->colourTable[num_bytes+1] = *green;
    		newImage->colourTable[num_bytes+2] = *blue;
    		
    		num_bytes= num_bytes + 3;
    	
			}
    	
    	
	}
	
	if(effect == "redTint"){
    	
    	while(num_bytes < 768){
    		*red = newImage->colourTable[num_bytes];
    		
    		*green = newImage-> colourTable[num_bytes+1];

    		*blue = newImage-> colourTable[num_bytes+2];
    		
    		toRedTint(red, green, blue);
    		
    		newImage->colourTable[num_bytes] = *red;
    		newImage->colourTable[num_bytes+1] = *green;
    		newImage->colourTable[num_bytes+2] = *blue;
    		
    		num_bytes= num_bytes + 3;
    	
			}
    	
    	
	}
	
		if(effect == "sepia"){
    	
    	while(num_bytes < 768){
    		
    		*red = newImage->colourTable[num_bytes];
    		
    		*green = newImage-> colourTable[num_bytes+1];

    		*blue = newImage-> colourTable[num_bytes+2];
    		
    		toSepia(red, green, blue);
    		
    		newImage->colourTable[num_bytes] = *red;
    		newImage->colourTable[num_bytes+1] = *green;
    		newImage->colourTable[num_bytes+2] = *blue;
    		
    		num_bytes= num_bytes + 3;
			}
    	
    	
	}
	
		if(effect == "brighten"){
    	
    	while(num_bytes < 768){
    		
    		*red = newImage->colourTable[num_bytes];
    		
    		*green = newImage-> colourTable[num_bytes+1];

    		*blue = newImage-> colourTable[num_bytes+2];
    		
    		brighten(red, green, blue,200);
    		
    		newImage->colourTable[num_bytes] = *red;
    		newImage->colourTable[num_bytes+1] = *green;
    		newImage->colourTable[num_bytes+2] = *blue;
    		
    		num_bytes= num_bytes + 3;
    	
			}
    	
    	
	}
	
	delete(red);
	delete(green);
	delete(blue);
    
    return newImage;
}



Animation * initAnimation () {
    Animation * newAnim;
    
    newAnim = new Animation;
    
    newAnim->numFrames = 0;
    
    return newAnim;
}



void addFrame (Animation * animation, Image * image, int duration) {
	
	AnimFrame frame;

	frame.image = image;
	frame.duration = duration;
	
	
	
	if(duration < 1 || duration > 255){
		cout << "Cannot add frame, the duration is too short or too long"<<endl;
		return;
	}
	
	int location = 0 + (animation->numFrames);
	
	//cout << "Location: " << location << ", NumFrames: " << animation->numFrames << endl;
	  // Ensure location is within bounds
	if (location >= 0 && location < 20) {
        animation->frames[location] = frame;
        animation->numFrames++;
    } else{
    	cout << "Invalid Location" <<endl;
    	return;
	}

	
	
}
    


void saveAnimation (Animation * animation, char filename[]) {

    unsigned char endByte = 59;
    unsigned char lsd[7]= {88, 2, 144, 1, 112, 0, 0};
    unsigned char header[6]= {71, 73, 70, 56, 57, 97};
    unsigned char appExt[19]= {33, 255, 11, 78, 69, 84, 83, 67, 65, 80, 69, 50, 46, 48, 3, 1, 0, 0, 0};
    unsigned char gcExt[8]= {33, 249, 4, 8, 100, 0, 0, 0};
    
    ofstream output;
    
      output.open(filename, ios::binary | ios::out);
    
    if(!output){
    	cout << "Error in opening " << filename << " file. Aborting ... " << endl;
    	exit(1);
	}
    // write bytes from header
    
    int header_size = sizeof(header)/ sizeof(header[0]);
    	
    	int i;
    	for(i=0; i<6 ; i++){
    		output.write((char*) &header[i], sizeof(header[i]));
		}
    
    // write bytes from lsd
    
     int lsd_size = sizeof(lsd)/ sizeof(lsd[0]);
    	
    	int j;
    	for(j=0; j<7 ; j++){
    		output.write((char*) &lsd[j], sizeof(lsd[j]));
		}
    
    
    // write bytes from application extension
    int appext_size = sizeof(appExt)/ sizeof(appExt[0]);
    	
    	int p;
    	for(p=0; p<19 ; p++){
    		output.write((char*) &appExt[p], sizeof(appExt[p]));
		}
     

    
    // write bytes for each frame
    
    int l=0;
    int num_frames= animation->numFrames;
    
    while(l<num_frames){
    	
    	gcExt[3] = animation->frames[l].duration;
    	int gcext_size = sizeof(gcExt)/ sizeof(gcExt[0]);
    	
    	int d;
    	for(d=0; d<8 ; d++){
    		output.write((char*) &gcExt[d], sizeof(unsigned char));//sizeof(gcExt[d]));
		}
     	
     	
    	
    	int r;
    	for(r=0; r<10 ; r++){
    	output.write((char*)&animation->frames[l].image->descriptor[r], sizeof(unsigned char));//sizeof(animation->frames[l].image->descriptor[r]));
		}
    	
    	
    	int f;
    	for(f=0; f<768 ; f++){
    	output.write((char*)&animation->frames[l].image->colourTable[f], sizeof(unsigned char));//sizeof(animation->frames[l].image->descriptor[f]));
		}
   	 	
		
		int a;
    	for(a=0; a < (animation->frames[l].image->sizeData) ; a++){
    	output.write((char*) &animation->frames[l].image->data[a], sizeof(unsigned char));//sizeof(animation->frames[l].image->data[a]));
	}
    	
		l++;
    	//num_frames--;
	}
    
    
    // write terminating byte
    output.write((char*)&endByte, sizeof(endByte));
    
    output.close();
    
    
}



// This function displays a Web page on the default browser (Windows only)

void showWebPage (string URL) {
    char cURL [50];
    int i;

    for (i = 0; i < URL.length(); i=i+1){
        cURL[i] = URL[i];
    }

    cURL[URL.length()] = '\0';
    
    ShellExecute(NULL, "open", cURL, NULL, NULL, SW_SHOWNORMAL);
}



int main() {

    Animation * scenery;
    Animation * random;

    Image * img1;
    Image * img2;
    Image * img3;
    Image * img4;
    Image * img5;

    Image * imgEff1;
    Image * imgEff2;
    Image * imgEff3;
    Image * imgEff4;
    Image * imgEff5;
    
    char webPage[50];
    strcpy (webPage, "Assignment3.html");    // copy Assignment3.html to folder with C++ program
    
    // Operation 1-create images for each GIF file
    
    char file[25];
    
    strcpy(file,"Saskatoon.gif");
    img1 = readImage(file);
    
    strcpy(file,"MoraineLake.gif");
    img2 = readImage(file);
    
    strcpy(file,"Maracas.gif");
    img3 = readImage(file);
    
    strcpy(file,"ScarletIbis.gif");
    img4 = readImage(file);
    
    strcpy(file,"PigeonPoint.gif");
    img5 = readImage(file);
    
    

    // Operation 2 to 6-apply effects to images and save to new GIF files
    char saks_filename[25]= "Saskatoon-GS.gif";
    char moraine_filename[25]= "MoraineLake-BT.gif";
    char maracas_filename[25]= "Maracas-S.gif";
    char scarlet_filename[25]= "ScarletIbis-B.gif";
    char pigeon_filename[25]= "PigeonPoint-RT.gif";
      
       char scenery_anim_filename[25]= "Scenery-Animation.gif";
    
    imgEff1 = applyEffectImage(img1, "grayScale");
    saveImage(imgEff1, saks_filename);
    
    imgEff2 = applyEffectImage(img2, "blueTint");
    saveImage(imgEff2, moraine_filename);
    
    imgEff3 = applyEffectImage(img3, "sepia");
    saveImage(imgEff3, maracas_filename);
    
    imgEff4 = applyEffectImage(img4, "brighten");
    saveImage(imgEff4, scarlet_filename );
    
    imgEff5 = applyEffectImage(img5, "redTint");
    saveImage(imgEff5, pigeon_filename);
    
    // Operation 7-create scenery animation
    
    scenery = initAnimation ();
    
    // Operation 8 to 12-add frames to scenery animation (250 duration for each frame)
    
    addFrame(scenery,img3,250);
    addFrame(scenery,imgEff2,250);
    addFrame(scenery,img5,250);
    addFrame(scenery,imgEff3,250);
    addFrame(scenery,img1,250);
    
//    for (int i=0 ; i<5; i++){
//    	cout << scenery->frames[i].duration << endl;
//	}
    

    // Operation 13-save scenery animation to GIF file
    saveAnimation(scenery, scenery_anim_filename );
    
    


    // Operation 14-create random animation
    
	random = initAnimation ();
    
    // Operation 15-add random frames to random animation (random duration between 50 and 255)
    
    Image * images[11];
    
    images[1] = img1;
    images[2] = img2;
    images[3] = img3;
    images[4]= img4;
	images[5] = img5;
	images[6] =  imgEff1;
	images[7] =  imgEff2;
	images[8] =  imgEff3;
	images[9] =  imgEff4;
	images[10] =  imgEff5;
	
	int ran;
	int ran_duration;
	srand((time(NULL)));
	
	for(int j=0 ; j<7 ; j++){
		
		ran = 1+rand()%10;
		ran_duration = 50 + rand()%(255-50+1);
		addFrame(random,images[ran], ran_duration);
	}
	
	 
//    for (int j=0 ; j<7; j++){
//    	cout << random->frames[j].duration << endl;
//	} // to check if the frames were enetered succesfully 
    
    
    char random_anim_filename[25] = "Random-Animation.gif";
    
    // Operation 16-save random animation to GIF file
    
    saveAnimation(random, random_anim_filename);
    
    
    // Display GIFs on a Web browser (Windows only)
    
    showWebPage(webPage);
     
      return 0;
}


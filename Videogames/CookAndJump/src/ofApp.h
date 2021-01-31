#pragma once

#include "ofMain.h"

//#define NUITRACK

#include <nuitrack/Nuitrack.h>
#include "ofxNuitrack.h"

#include "ofxGLWarper.h"
#include "ofxUI.h"
#include "ofxXmlSettings.h"
#include "ofxTimer.h"

#include "DEFINES.h"
#include "Singleton.h"
#include "Burger.h"

#include "grid/grid.h"
#include "botons/botoDonut.h"
#include "peces/pecaEmpty.h"


using namespace tdv::nuitrack;

#ifdef NUITRACK
class Bone {
public:
	Bone(JointType _from, JointType _to, glm::vec3 _direction) {
		from = _from;
		to = _to;
		direction = _direction;
	}

	JointType from;
	JointType to;
	glm::vec3 direction;
};
#endif

class ofApp: public ofBaseApp{
	public:
		
		void setup();
		void update();
		void draw();
		void exit();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

#ifdef NUITRACK		
		//--------------------------------------------------------------
		// REALSENSE
		void updatePointcloud();
		void updateJoint(Joint &j);
		void drawSkeleton(Skeleton &s);
		void drawJoint(Joint &j);
		void drawBones(vector<Joint> joints);
		void drawUsers();
		void drawPointcloud();

		bool bNeedPointcloudUpdate;
		ofxnui::TrackerRef tracker;
		ofTexture rgbTex;
		ofTexture depthTex;		
		glm::vec2 rgbFrameSize;
		glm::vec2 depthFrameSize;

		vector<Skeleton> skeletons;
		vector<User> users;

		ofVboMesh pc;				// pointcloud

		float cameraHeight = 1.0;	// height of camera in realspace (m)
		glm::vec3 floorPoint;
		glm::vec3 floorNormal;
#endif

                //--------------------------------------------------------------
                // APP
                Singleton *singleton; // PUNTUACIÓ jugador

                pantallesJoc pantallaJoc;
                string pantallaToString();

                void setVariablesIniciPartida(); // endreçat
                void drawPuntuacio();
                void drawEnd();

                // CORNER PIN
                ofxGLWarper warper;

                // BOTONS
                botoDonut botoStart;
				pecaEmpty end;
				pecaEmpty plate;
				pecaEmpty trash;

                // GRID
                grid myGrid;
				Burger order;

				int level;
				int money;

                // TEMPS DE JOC
                ofxTimer jocMinutsTimer;
                float jocMinutsTimerSegonsLeft;
                int jocMinutsTimerMinuts;
                int jocMinutsTimerSegons;
                void drawTemps();
				ofColor saltingBlue;
				ofColor black;
                ofTrueTypeFont	saltingTypo;

                // TEMPS DE PANTALLA FINAL
                ofxTimer duradaTheEndTimer;

                //--------------------------------------------------------------
                // PUNTER
                ofImage punter;
                float punterWidthMig, punterHeightMig;

                // PECES
                int comptadorPeces;
                void setupPeca();
                void comprobarEstatsPecesEmpty();
                void actualitzaPuntsEmpty(int & e);
				
				int vides; //vides que te el jugador

				//ALIMENTS
				pecaEmpty pa_baix;
				pecaEmpty pa_dalt;
				pecaEmpty meat;
				pecaEmpty tomato;
				pecaEmpty cheese;
				pecaEmpty enciam;
				pecaEmpty onion;
				pecaEmpty bacon;
				pecaEmpty ketchup;
				
				int ultimaPecaIndex;
				
				std::vector<pecaEmpty> aliments;

				std::vector<int> clientOrder;
				std::vector<int> playerOrder;

				JointType typeBlobs[MAX_NUM_BLOBS];
                //--------------------------------------------------------------
                // DETECCIÓ
                float relAspectWidth; // detecció
                float relAspectHeight;
                float baixaHoTotAvall; // ajust fi amunt-avall
                float mouHoTotDretaEsq; // ajust fi esquerra-dreta

                bool bshowImagesAndSkeleton = false;
                bool bshowCamera = false;

                // contorns
                float tmpX, tmpY;
                ofVec4f tmpVecBlobPos;
                ofVec2f warpMousePos;
				JointType warpMouseType = JOINT_LEFT_HAND;

                //blobs
                ofVec2f posicionsBlobs[MAX_NUM_BLOBS];
				JointType tipusBlobs[MAX_NUM_BLOBS];
                int totalBlobsDetected;

                // GUI APP i DETECCIO
                ofxUICanvas *guia;

                // GUI AJUST BOTONS
                ofxUICanvas *guib;

                // GUI HELP
                ofxUICanvas *guih;
                void guiEvent(ofxUIEventArgs &e); // per a tots els GUIs

                //--------------------------------------------------------------
                // HELP INFO
                void toogleDrawInfoGrid();
                bool bdrawMouseWarped = true;

				//BACKGROUND
				ofImage bg_start;
				ofImage bg_play_5;
				ofImage bg_play_4;
				ofImage bg_play_3;
				ofImage bg_play_2;
				ofImage bg_play_1;
				ofImage bg_end;

				//SOUNDS
				ofSoundPlayer chopping_sound;
				ofSoundPlayer new_order_sound;
				ofSoundPlayer trash_sound;
				ofSoundPlayer lettuce_sound;
				ofSoundPlayer cobrar_sound;
				ofSoundPlayer cooking;
};

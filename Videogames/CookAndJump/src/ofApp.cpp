#include "ofApp.h"
#include <cassert>

//#define OPEN_MP_TEST
// comment out to enable OpenMP
// also need turnning on openMP support from visual studio project properties

//#define NUITRACK

#ifdef OPEN_MP_TEST
#include <omp.h>
#endif

#ifdef NUITRACK
using namespace tdv::nuitrack;
#endif


using namespace glm;
//--------------------------------------------------------------
void ofApp::setup(){

       // APP
        ofSetFullscreen(false);
        //ofHideCursor();

		level = 1;
		money = 0;
		vides = 5;
		//BACKGROUNDS
		bg_start.load("tga/background_start.tga");
		bg_play_5.load("tga/background_grey_5.tga");
		bg_play_4.load("tga/background_grey_4.tga");
		bg_play_3.load("tga/background_grey_3.tga");
		bg_play_2.load("tga/background_grey_2.tga");
		bg_play_1.load("tga/background_grey_1.tga");
		bg_end.load("tga/background_end.tga");

		//SOUNDS
		chopping_sound.load("sounds/chopping.wav");
		chopping_sound.setVolume(0.75);
		new_order_sound.load("sounds/new_order.wav");
		new_order_sound.setVolume(0.25);
		trash_sound.load("sounds/trash.wav");
		trash_sound.setVolume(0.25);
		lettuce_sound.load("sounds/lettuce.wav");
		lettuce_sound.setVolume(0.75);
		cobrar_sound.load("sounds/cobrar.wav");
		cobrar_sound.setVolume(0.25);
		cooking.load("sounds/propizio_chitarra.wav");
		cooking.setVolume(0.75);
		cooking.play();

        // GENRAL
        ofSetVerticalSync(true);
        ofSetCircleResolution(300);
        ofTrueTypeFont::setGlobalDpi(72);
        singleton = Singleton::getInstance();

        // TEMPS DE JOC
        jocMinutsTimer.setup(3*60*1000, false); // 3 minuts = 3*60*1000 ms
        jocMinutsTimer.stopTimer();

        duradaTheEndTimer.setup(5*1000, false); // 5 segons
        duradaTheEndTimer.stopTimer();

		// WARPER
		warper.setup(0,0, APP_WIDTH,APP_HEIGT);
		warper.load("TrampoLima_CornerPin.xml");
		warper.enableKeys(true);
		warper.deactivate();

		// PUNTER
		punter.load("punter.png");
		punterWidthMig = punter.getWidth()*0.5;
		punterHeightMig = punter.getHeight()*0.5;

        // COLOR JOC
		saltingBlue = ofColor(0, 192, 255);
		black = ofColor(0,0,0);

        // TYPO
        ofTrueTypeFont::setGlobalDpi(72);
        saltingTypo.load("Oswald-Bold.otf", 27, true, true); // temps nivell i normal, kids, pro
        saltingTypo.setLetterSpacing(1.2);

        // PECES

        // BOTONS
        guib = new ofxUICanvas(20, 20, APP_WIDTH*0.3, APP_HEIGT*0.9);

        guib->addLabel(":: BOTONS ::", OFX_UI_FONT_MEDIUM);
        guib->addSpacer();
        guib->addIntSlider("botoStartX", 0, APP_WIDTH, &botoStart.botoX)->setIncrement(1);
        guib->addIntSlider("botoStartY", 0, APP_HEIGT, &botoStart.botoY)->setIncrement(1);

        botoStart.setup(botoStart.botoX,botoStart.botoY, 80, saltingBlue);

        guib->autoSizeToFitWidgets();
        ofAddListener(guib->newGUIEvent,this,&ofApp::guiEvent);
        guib->loadSettings("TrampoLima_Botons.xml");

        guib->setVisible(false);

        // GRID
        myGrid.setup();
        myGrid.gridActivaExcepteMargesSupInfDretaEsq();

		//ALIMENTS
		comptadorPeces = 0;
		
		pa_baix.setup(0, 14, myGrid.returnPosicioOfPeca(14), 50);
		pa_baix.icon.load("tga/pa_baix.tga");
		aliments.push_back(pa_baix);
		
		pa_dalt.setup(1, 20, myGrid.returnPosicioOfPeca(20), 50);
		pa_dalt.icon.load("tga/pa_dalt.tga");
		aliments.push_back(pa_dalt);
		
		meat.setup(2, 10, myGrid.returnPosicioOfPeca(10), 50);
		meat.icon.load("tga/meat.tga");
		aliments.push_back(meat);
		
		tomato.setup(3, 11, myGrid.returnPosicioOfPeca(11), 50);
		tomato.icon.load("tga/tomato.tga");
		aliments.push_back(tomato);
		
		cheese.setup(4, 9, myGrid.returnPosicioOfPeca(9), 50);
		cheese.icon.load("tga/cheese.tga");
		aliments.push_back(cheese);
		
		enciam.setup(5, 27, myGrid.returnPosicioOfPeca(27), 50);
		enciam.icon.load("tga/lettuce.tga");
		aliments.push_back(enciam);
		
		onion.setup(6, 8, myGrid.returnPosicioOfPeca(8), 50);
		onion.icon.load("tga/onion.tga");
		aliments.push_back(onion);
		
		ketchup.setup(7, 21, myGrid.returnPosicioOfPeca(21), 50);
		ketchup.icon.load("tga/ketchup.tga");
		aliments.push_back(ketchup);

		ofAddListener(pecaEmpty::actualitzaPunts, this, &ofApp::actualitzaPuntsEmpty);

		order.createOrder( level, clientOrder, aliments );//We create the order

		trash.setup(8, 36, myGrid.returnPosicioOfPeca(36), 30);
		trash.icon.load("tga/trash.tga");
		plate.setup(9, 38, myGrid.returnPosicioOfPeca(38), 30);
		plate.icon.load("tga/plate.tga");
		end.setup(9, 40, myGrid.returnPosicioOfPeca(40), 30);
		end.icon.load("tga/end.tga");

		ultimaPecaIndex = -1; //Definim inicialment com a -1

        // GUI APP
        guia = new ofxUICanvas(20, 20, APP_WIDTH*0.4, APP_HEIGT*0.9);

        guia->addLabel(":: TRAMPOLIMA SETTINGS ::", OFX_UI_FONT_MEDIUM);
        guia->addSpacer();

        guia->addSpacer();
        guia->addLabelButton("load factory defaults", false);

        guia->addSpacer();
        guia->addLabel("GAME IMAGE CORNERS", OFX_UI_FONT_MEDIUM);
        guia->addLabelButton("reset corners", false);

        guia->addSpacer();
        guia->addLabel("4 GAME CONTROLLERS POSITION", OFX_UI_FONT_MEDIUM);
        guia->addSlider("adjustment up-down", -1.5*APP_WIDTH, 1.5*APP_WIDTH, &baixaHoTotAvall);
        guia->addSlider("adjustment left-right", -1.5*APP_WIDTH, 1.5*APP_WIDTH, &mouHoTotDretaEsq);

        guia->autoSizeToFitWidgets();
        ofAddListener(guia->newGUIEvent,this,&ofApp::guiEvent);
        guia->loadSettings("TrampoLima_Deteccio.xml");

        guia->setVisible(false);

        // HELP
        guih = new ofxUICanvas(20, 20, APP_WIDTH*0.7, APP_HEIGT*0.9);

        guih->addLabel(":: HELP ::", OFX_UI_FONT_MEDIUM);
        guih->addSpacer();
        guih->addFPS();
        guih->addSpacer();
        guih->addTextArea("helpText1", "step 1) WARP: press W | w and mouse click and drag to adjust game's image corners");
        guih->addTextArea("helpText2", "step 2) CAM: press C | c to show and hide camera image");
        guih->addTextArea("helpText3", "step 3) GAME: press J | j | G | g to adjust game");
        guih->addTextArea("helpText5", "step 4) BUTTONS & GRID: press B | b to show and hide grid and buttons adjustments");

        guih->autoSizeToFitWidgets();
        ofAddListener(guih->newGUIEvent,this,&ofApp::guiEvent);

        guih->setVisible(false);

        // PARTIDA
        setVariablesIniciPartida();

#ifdef NUITRACK
	// REALSENSE
    bool bPlayFile = false;
	pc.setMode(OF_PRIMITIVE_POINTS);

	tracker = ofxnui::Tracker::create();
	tracker->init();
	tracker->setConfigValue("Realsense2Module.Depth.FPS", "30");
	//tracker->setConfigValue("Realsense2Module.Depth.RawWidth", "1280");
	//tracker->setConfigValue("Realsense2Module.Depth.RawHeight", "720");
	//tracker->setConfigValue("Realsense2Module.Depth.ProcessWidth", "1280");
	//tracker->setConfigValue("Realsense2Module.Depth.ProcessHeight", "720");
	tracker->setConfigValue("Realsense2Module.Depth.ProcessMaxDepth", "7000");
	tracker->setConfigValue("DepthProvider.RotateAngle", "90");

	tracker->setConfigValue("Realsense2Module.RGB.FPS", "30");
	//tracker->setConfigValue("Realsense2Module.RGB.RawWidth", "1280");
	//tracker->setConfigValue("Realsense2Module.RGB.RawHeight", "720");
	//tracker->setConfigValue("Realsense2Module.RGB.ProcessWidth", "1280");
	//tracker->setConfigValue("Realsense2Module.RGB.ProcessHeight", "720");

	// post processing setting
	tracker->setConfigValue("Realsense2Module.Depth.PostProcessing.SpatialFilter.spatial_alpha", "0.1");
	tracker->setConfigValue("Realsense2Module.Depth.PostProcessing.SpatialFilter.spatial_delta", "50");

	if(bPlayFile){
	    string path = ofToDataPath("video.bag", true); // must be absolute path
	    tracker->setConfigValue("Realsense2Module.FileRecord", path);
	}

	tracker->setConfigValue("Segmantation.MAX_DISTANCE", "10000");
	tracker->setConfigValue("Skeletonization.MaxDistance", "10000");

	tracker->setIssuesCallback([this](IssuesData::Ptr data) {
		auto issue = data->getIssue<Issue>();
		if (issue) {
			ofLogNotice() << "Issue detected! " << issue->getName() << " [" << issue->getId() << "]";
		}
	});

	tracker->setRGBCallback([this](RGBFrame::Ptr data) {
		rgbFrameSize.x = data->getCols();
		rgbFrameSize.y = data->getRows();
		const Color3 *colorData = data->getData();
		uint8_t *colorDataPtr = (uint8_t *)colorData;
		ofPixels pix;

		string fileName = tracker->getConfigValue("Realsense2Module.FileRecord");
		if (fileName == "") {
			//Live feed = BGR
			pix.setFromPixels(colorDataPtr, rgbFrameSize.x, rgbFrameSize.y, OF_PIXELS_BGR);
		}
		else{
			// BAG file = RGB
			pix.setFromPixels(colorDataPtr, rgbFrameSize.x, rgbFrameSize.y, OF_PIXELS_RGB);
		}
		rgbTex.loadData(pix);
	});

	tracker->setDepthCallback([this](DepthFrame::Ptr data) {
		depthFrameSize.x = data->getCols();
		depthFrameSize.y = data->getRows();
		const unsigned short *depthData = data->getData();
		ofShortPixels pix;
		pix.setFromPixels(depthData, depthFrameSize.x, depthFrameSize.y, OF_PIXELS_GRAY);
		depthTex.loadData(pix);
		bNeedPointcloudUpdate = true;
	});


	tracker->setSkeletonCallback([this](SkeletonData::Ptr data) {
		skeletons = data->getSkeletons();
	});

	tracker->setUserCallback([this](UserFrame::Ptr data) {
		floorPoint = ofxnui::Tracker::fromVector3(data->getFloor()) / vec3(1000);
		floorNormal = ofxnui::Tracker::fromVector3(data->getFloorNormal());
		users = data->getUsers();
	});

	tracker->run();
#endif
}

//--------------------------------------------------------------
void ofApp::setVariablesIniciPartida(){
    // APP
    singleton->setPuntuacioJugador(0);

    // BOTONS
	botoStart.botoSeleccionat = false;

    // TEMPS DE JOC
    jocMinutsTimer.reset();
    jocMinutsTimer.stopTimer();
    duradaTheEndTimer.reset();
    duradaTheEndTimer.stopTimer();

}

//--------------------------------------------------------------
void ofApp::update(){
#ifdef NUITRACK
  // TRACKER --------------------------------------------
  tracker->poll();
  updatePointcloud();

  totalBlobsDetected = 0; // s'actualitza dins el updateJoint
  for(int i=0; i<skeletons.size(); i++) {
    auto s = skeletons[i];
    auto joints = s.joints;
    for (int n=0; n<joints.size(); n++) {
      auto j = joints[n];
      updateJoint(j);
    }
  }
#endif

  for(int i=0; i<totalBlobsDetected; i++) {
    posicionsBlobs[i].x = posicionsBlobs[i].x + mouHoTotDretaEsq;
    posicionsBlobs[i].y = posicionsBlobs[i].y + baixaHoTotAvall;
  }

  // PANTALLES ------------------------------------------

  if (pantallaJoc == START) {
	  botoStart.update(totalBlobsDetected, posicionsBlobs);
	  botoStart.updatem(warpMousePos);

	  if (botoStart.botoSeleccionat == true) { // CANVI A pantallaJoc = PLAY;
		  pantallaJoc = PLAY;
		  jocMinutsTimer.startTimer();

		  setupPeca();
	  }

  }
  else if (pantallaJoc == PLAY) {

	  end.update(totalBlobsDetected, posicionsBlobs, typeBlobs);
	  end.updatem(warpMousePos, warpMouseType);
	  if (end.estatPeca == TOCADA) {
		  if (order.checkOrder(clientOrder, playerOrder)) {
			  singleton->setPuntuacioJugador(singleton->getPuntuacioJugador() + 1);
			  money = money + (25 * level);
			  cobrar_sound.play();
			  float aux = singleton->getPuntuacioJugador() * 0.5;
			  level = ceil(aux); //Cada dos punts, s'augmenta un nivell
			  printf("%d", level);
			  clientOrder.clear();
			  playerOrder.clear();
			  order.createOrder(level, clientOrder, aliments);
			  new_order_sound.play();
		  }
		  else {
			  vides--;
			  playerOrder.clear();
			  printf("*** MALAMENT ***\n");
		  }
	  }
	  
	  plate.update(totalBlobsDetected, posicionsBlobs, typeBlobs);
	  plate.updatem(warpMousePos, warpMouseType);
	  for (int i = 0; i < aliments.size(); i++) {
		  aliments[i].update(totalBlobsDetected, posicionsBlobs, typeBlobs);
		  aliments[i].updatem(warpMousePos, warpMouseType);
		  comprobarEstatsPecesEmpty();
	  }

	  if (aliments[3].estatPeca == TOCADA || aliments[6].estatPeca == TOCADA) {
		  chopping_sound.play();
	  }
	  if (aliments[5].estatPeca == TOCADA) {
		  lettuce_sound.play();
	  }

	  //Actualitza la l'index de ultima peca tocada
	  order.lastSelected(aliments, ultimaPecaIndex);

	  if (vides == 0 || level == 5) {
		  pantallaJoc = END;
	  }

	  trash.update(totalBlobsDetected, posicionsBlobs, typeBlobs);
	  trash.updatem(warpMousePos, warpMouseType);
	  if (trash.estatPeca == TOCADA) {
		  trash_sound.play();
		  playerOrder.clear();
	  }
  }
  else if (pantallaJoc == END) {
	  if (duradaTheEndTimer.isTimerFinished()) {
		  setVariablesIniciPartida();
		  pantallaJoc = START;
	  }
  }

  // TEMPS ----------------------------------------------
  jocMinutsTimerSegonsLeft = jocMinutsTimer.getTimeLeftInSeconds();
  jocMinutsTimerMinuts = (int)jocMinutsTimerSegonsLeft / 60;
  jocMinutsTimerSegons = jocMinutsTimerSegonsLeft - jocMinutsTimerMinuts * 60;

  /*if (jocMinutsTimer.isTimerFinished()) {
	  pantallaJoc = END;
	  jocMinutsTimer.reset();
	  jocMinutsTimer.stopTimer();
	  duradaTheEndTimer.startTimer();
  }*/
}

#ifdef NUITRACK
//--------------------------------------------------------------
void ofApp::updatePointcloud() {
	if (bNeedPointcloudUpdate) {

		DepthSensor::Ptr dtracker = tracker->depthTracker;
		DepthFrame::Ptr dframe = dtracker->getDepthFrame();
		int row = dframe->getRows();
		int col = dframe->getCols();
		int skip = 4;	// downsampling level, 1 = max
		int size = ((float)row/skip) * ceil((float)col / skip);

		// allocate vertices and colors only once
		if (pc.getVertices().size() == 0) {
			pc.clear();

			vector<glm::vec3> p;
			p.assign(size, glm::vec3(0,0,0));
			pc.addVertices(p);

			vector<ofFloatColor> c;
			c.assign(size, ofFloatColor(0,0,0,0.9));
			pc.addColors(c);
		}

		const unsigned short * data = dframe->getData();

#ifdef OPEN_MP_TEST
		#pragma omp parallel for num_threads(4)
#endif
		for (int y = 0; y < row; y += skip) {
			for (int x = 0; x < col; x += skip) {
				int index = y * col + x;
				int skippedIndex = (y/skip) * (col/skip) + (x/skip);
				unsigned short d = data[index];
				Vector3 v = dtracker->convertProjToRealCoords(x, y, d);
				pc.setVertex(skippedIndex, glm::vec3(v.x, v.y, v.z)*0.001);
				if (v.y>-900) {
					float g = ofMap(d, 0, 5000, 0.1, 1.0, true);
					pc.setColor(skippedIndex, ofFloatColor(g, g, 0, 0.9)); // groc
				}
				else {
					pc.setColor(skippedIndex, ofFloatColor(0, 0, 0, 0));
				}
			}
		}
		
		bNeedPointcloudUpdate = false;
	}
}
#endif

//--------------------------------------------------------------
void ofApp::draw(){
    

	// WARP begin
	warper.begin();
	warper.draw();

	// APP
	ofBackground(255);
	ofSetColor(255);
	ofSetWindowTitle("TrampoLima running at " + ofToString((int)ofGetFrameRate()) + " frames per second");

	// FONS
	/*ofSetColor(155);
	ofFill();
	ofDrawRectangle(0,0, APP_WIDTH, APP_HEIGT);*/

	bg_start.resize(APP_WIDTH, APP_HEIGT);
	bg_play_5.resize(APP_WIDTH, APP_HEIGT);
	bg_play_4.resize(APP_WIDTH, APP_HEIGT);
	bg_play_3.resize(APP_WIDTH, APP_HEIGT);
	bg_play_2.resize(APP_WIDTH, APP_HEIGT);
	bg_play_1.resize(APP_WIDTH, APP_HEIGT);
	bg_end.resize(APP_WIDTH, APP_HEIGT);

	// PANTALLES --------------------------------------------------------------------------
	if (pantallaJoc == START) {
		bg_start.draw(0, 0);
		botoStart.draw();

	}
	else if (pantallaJoc == PLAY) {

		if (vides == 5) {
			bg_play_5.draw(0, 0);
			order.drawOrder(aliments, myGrid, clientOrder);
		}
		else if (vides == 4) {
			bg_play_4.draw(0, 0);
			order.drawOrder(aliments, myGrid, clientOrder);
		}
		else if (vides == 3) {
			bg_play_3.draw(0, 0);
			order.drawOrder(aliments, myGrid, clientOrder);
		}
		else if (vides == 2) {
			bg_play_2.draw(0, 0);
			order.drawOrder(aliments, myGrid, clientOrder);
		}
		else if (vides == 1) {
			bg_play_1.draw(0, 0);
			order.drawOrder(aliments, myGrid, clientOrder);
		}

		ofVec2f end_pos = ofVec2f(myGrid.returnPosicioOfPeca(end.pecaNumGraella).x - 50, myGrid.returnPosicioOfPeca(end.pecaNumGraella).y - 50);
		end.icon.resize(100, 100);
		end.icon.draw(end_pos);

		ofVec2f plate_pos = ofVec2f(myGrid.returnPosicioOfPeca(plate.pecaNumGraella).x - 50, myGrid.returnPosicioOfPeca(plate.pecaNumGraella).y - 20);
		plate.icon.resize(100, 100);
		plate.icon.draw(plate_pos);

		ofVec2f trash_pos = ofVec2f(myGrid.returnPosicioOfPeca(trash.pecaNumGraella).x - 50, myGrid.returnPosicioOfPeca(trash.pecaNumGraella).y - 50);
		trash.icon.resize(100, 100);
		trash.icon.draw(trash_pos);

		order.drawPlayerOrder(aliments, myGrid, playerOrder);

		//drawTemps();
		drawPuntuacio();

		end.draw();
		plate.draw();
		trash.draw();

		for (int i = 0; i < aliments.size(); i++) {
			aliments[i].draw();
		}

		//Draw the order in the top right corner of the screen
		comprobarEstatsPecesEmpty();

		for (int i = 0; i < aliments.size(); i++) {
			if (aliments[i].isSelected == true) { //Si l'aliment s'ha seleccionat pero encara no s'ha posat al plat
				if (plate.estatPeca == TOCADA && ultimaPecaIndex != -1) { //Si s'ha colocat al plat
					aliments[i].isPositioned = true;
					aliments[i].isSelected = false;
					playerOrder.push_back(ultimaPecaIndex);

				}
			}
		}
	}
	else if (pantallaJoc == END) {
		drawEnd();
	}

	// HELP ----------------------------------------------------
	// GRID
	myGrid.draw();
	if (bdrawMouseWarped) {
		ofSetColor(255, 255, 0);
		ofDrawCircle(warpMousePos.x, warpMousePos.y, 5, 5);
	}
	//ofSetColor(255,255,0);
	//ofDrawBitmapString("ESTAT: " + pantallaToString(), 20,20);

	// PUNTERS ----------------------------------------------------
	ofSetColor(255);
	for (int i = 0; i < totalBlobsDetected; i++) {
		punter.draw(posicionsBlobs[i].x - punterWidthMig, posicionsBlobs[i].y - punterHeightMig);
	}

#ifdef NUITRACK
    // CAMERA
    if(bshowCamera){
        ofSetColor(255);
        if (rgbTex.isAllocated()) {
          rgbTex.draw(0,0,APP_WIDTH,APP_HEIGT);
        }
    }
#endif

    // WARP end
    warper.end();

#ifdef NUITRACK
    // INFO ------------------------------------------------------------------------
    // CAMERES & STICKMAN & PRINTS
    if(bshowImagesAndSkeleton){
        float camWidthPorc = rgbFrameSize.x*0.25;
        float camHeightPorc = rgbFrameSize.y*0.25;
        ofSetColor(255);
        if (rgbTex.isAllocated()) {
          rgbTex.draw(400,20, camWidthPorc,camHeightPorc);
        }
        if (depthTex.isAllocated()) {
          depthTex.draw(400+camWidthPorc+20,20, camWidthPorc,camHeightPorc);
        }

        ofSetHexColor(0xffffff);
        ofDrawBitmapString("camera image", 400,10);
        ofDrawBitmapString("depth texture", 400+camWidthPorc+20,10);

        //drawPointcloud();
        ofDrawBitmapString(ofToString(skeletons.size()) + " skeletons", 400,20+camHeightPorc+20);
        for (int i = 0; i < skeletons.size(); i++) {
          auto s = skeletons[i];
          drawSkeleton(s);
        }
        drawUsers(); // // draw user's center of mass

        ofSetColor(255, 255, 0);
        ofDrawBitmapString("RGBFRAME SIZE " + ofToString(rgbFrameSize.x) + ",  " + ofToString(rgbFrameSize.y), 20, APP_HEIGT_MEITAT-20);
    }
#endif
}

#ifdef NUITRACK
void ofApp::drawUsers() {
  for (auto & u : users) {
    int uid = u.id;

    // REAL -----
    const Vector3 & v = u.real; // real pos is in mm
    ofVec3f vmap = ofVec3f(0, 0, 0);
    vmap.x = ofMap(v.x, -rgbFrameSize.x*0.5,rgbFrameSize.x*0.5, 0,APP_WIDTH);
    vmap.y = ofMap(v.y, rgbFrameSize.y*0.5,-rgbFrameSize.y*0.5, 0,APP_HEIGT);
    vmap.z = 0;
    ofVec4f vwarp = warper.fromScreenToWarpCoord(vmap.x,vmap.y,vmap.z);
    float radius = ofMap(v.z, 50,7000, 1,20);

    ofSetColor(255, 255, 0);
    string info = "CENTER OF MASS " + ofToString(static_cast<int>(v.x)) + ", " + ofToString(static_cast<int>(v.y));
    info += ", " + ofToString(static_cast<int>(v.z));
    ofDrawBitmapString(info, 20, APP_HEIGT_MEITAT);

    /*
    // draw user's center of mass
    ofPushMatrix();
    ofTranslate(vmap.x, vmap.y, vmap.z);
    ofFill();
    ofSetColor(255, 255, 0); // yellow
    ofDrawSphere(0,0,0,radius);
    ofDrawBitmapString("USER " + ofToString(uid), radius+5, radius+5);
    ofPopMatrix();

    // draw user's center of mass warped
    ofPushMatrix();
    ofTranslate(vwarp.x, vwarp.y, vwarp.z);
    ofFill();
    ofSetColor(0, 255, 255); // cian
    ofDrawSphere(0,0,0,radius);
    ofDrawBitmapString("USER " + ofToString(uid), radius+5, radius+5);
    ofPopMatrix();
    */

    // PROJ -----
    const Vector3 & p = u.proj;
    ofVec3f pmap = ofVec3f(0, 0, 0);
    pmap.x = ofMap(p.x, 0.0,1.0, 0,APP_WIDTH);
    pmap.y = ofMap(p.y, 0.0,1.0, 0,APP_HEIGT);
    pmap.z = 0;
    ofVec4f pwarp = warper.fromScreenToWarpCoord(pmap.x,pmap.y,pmap.z);
    BoundingBox bb = u.box;

    ofSetColor(255, 0, 255);
    info = "CENTER OF MASS " + ofToString(p.x) + ", " + ofToString(p.y);
    info += ", " + ofToString(p.z);
    ofDrawBitmapString(info, 20, APP_HEIGT_MEITAT+20);

    /*
    // draw user's center of mass
    ofPushMatrix();
    ofTranslate(pmap.x, pmap.y, pmap.z);
    ofFill();
    ofSetColor(255, 0, 255); // magenta
    ofDrawSphere(0,0,0,radius);
    ofDrawBitmapString("USER " + ofToString(uid), radius+5, radius+5);
    ofNoFill();
    float bbample = (bb.right - bb.left)*APP_WIDTH;
    float bbalt = (bb.bottom- bb.top)*APP_HEIGT;
    ofDrawRectangle(bb.top, bb.left, bbample, bbalt);
    ofPopMatrix();
    */

    // draw user's center of mass
    ofPushMatrix();
    ofTranslate(pwarp.x, pwarp.y, pwarp.z);
    ofFill();
    ofSetColor(255, 128, 0); // taronja
    ofDrawSphere(0,0,0,radius);
    ofDrawBitmapString("USER " + ofToString(uid), radius+5, radius+5);
    ofPopMatrix();
  }
}

void ofApp::drawSkeleton(Skeleton &s) {
  auto joints = s.joints;
  for (auto j : joints) {
          drawJoint(j);
  }
  drawBones(joints);
}

void ofApp::drawJoint(Joint &j) {
    if (j.type == JOINT_NONE) {
            return;
    }
    if (j.confidence < 0.15) {
            return;
    }

    // real pos is in mm, lets convert to m
    if(j.type==JOINT_HEAD||j.type==JOINT_LEFT_HAND||j.type==JOINT_RIGHT_HAND||
       j.type==JOINT_LEFT_ANKLE||j.type==JOINT_RIGHT_ANKLE){
        // REAL  -----
        const Vector3 & v = j.real;
        ofVec3f vmap = ofVec3f(0, 0, 0);
        vmap.x = ofMap(v.x, -rgbFrameSize.x*0.5,rgbFrameSize.x*0.5, 0,APP_WIDTH);
        vmap.y = ofMap(v.y, rgbFrameSize.y*0.5,-rgbFrameSize.y*0.5, 0,APP_HEIGT);
        vmap.z = 0;
        ofVec4f vwarp = warper.fromScreenToWarpCoord(vmap.x,vmap.y,vmap.z);
        float radius = ofMap(v.z, 50,7000, 1,50);

        /*
        // draw joint's center of mass
        ofPushMatrix();
        ofTranslate(vmap.x, vmap.y, vmap.z);
        ofFill();
        ofSetColor(255, 255, 0); // yellow
        ofDrawBox(0,0,0,radius);
        ofPopMatrix();

        // draw joint's center of mass warped
        ofPushMatrix();
        ofTranslate(vwarp.x, vwarp.y, vwarp.z);
        ofFill();
        ofSetColor(0, 255, 255); // cian
        ofDrawBox(0,0,0,radius);
        ofPopMatrix();
        */

        // PROJ -----
        const Vector3 & p = j.proj;
        ofVec3f pmap = ofVec3f(0, 0, 0);
        pmap.x = ofMap(p.x, 0.0,1.0, 0,APP_WIDTH);
        pmap.y = ofMap(p.y, 0.0,1.0, 0,APP_HEIGT);
        pmap.z = 0;
        pmap.x = APP_WIDTH-pmap.x; // TODO posr en un control aquest invertit per la pos de camera
        ofVec4f pwarp = warper.fromScreenToWarpCoord(pmap.x,pmap.y,pmap.z);

        /*
        // draw user's center of mass
        ofPushMatrix();
        ofTranslate(pmap.x, pmap.y, pmap.z);
        ofFill();
        ofSetColor(255, 0, 255); // magenta
        ofDrawBox(0,0,0,radius);
        ofDrawBitmapString(ofToString(pmap.x) + ", " + ofToString(pmap.y), 20,20);
        ofPopMatrix();
        */

        // draw user's center of mass
        ofPushMatrix();
        ofTranslate(pwarp.x, pwarp.y, pwarp.z);
        ofFill();
        ofSetColor(255, 128, 0); // taronja
        ofDrawBox(0,0,0,radius);
        ofDrawBitmapString(ofToString(pwarp.x) + ", " + ofToString(pwarp.y), 20,20);
        ofPopMatrix();
    }
}

void ofApp::updateJoint(Joint &j){
  if (j.type == JOINT_NONE) {
          return;
  }
  if (j.confidence < 0.15) {
          return;
  }

  if(j.type==JOINT_HEAD||j.type==JOINT_LEFT_HAND||j.type==JOINT_RIGHT_HAND||j.type==JOINT_LEFT_ANKLE||j.type==JOINT_RIGHT_ANKLE){
      // PROJ -----
      const Vector3 & p = j.proj;
      ofVec3f pmap = ofVec3f(0, 0, 0);
      pmap.x = ofMap(p.x, 0.0,1.0, 0,APP_WIDTH);
      pmap.y = ofMap(p.y, 0.0,1.0, 0,APP_HEIGT);
      pmap.z = 0;
      pmap.x = APP_WIDTH-pmap.x; // TODO posr en un control aquest invertit per la pos de camera
      posicionsBlobs[totalBlobsDetected].x = pmap.x;
	  posicionsBlobs[totalBlobsDetected].y = pmap.y;
	  tipusBlobs[totalBlobsDetected] = j.type;
      totalBlobsDetected++;
  }
}

void ofApp::drawBones(vector<Joint> joints) {
	//if (joints.size() < 3) {
	//	return;
	//}

	// rotation based on T pose
	// sdk reports rotations relative to T pose..
	static vector<Bone> bones = {
		Bone(JOINT_WAIST, JOINT_TORSO, vec3(0, 1, 0)),
		Bone(JOINT_TORSO, JOINT_NECK, vec3(0, 1, 0)),
		Bone(JOINT_NECK, JOINT_HEAD, vec3(0, 1, 0)),

		Bone(JOINT_LEFT_COLLAR, JOINT_LEFT_SHOULDER, vec3(1, 0, 0)),
		Bone(JOINT_LEFT_SHOULDER, JOINT_LEFT_ELBOW, vec3(1, 0, 0)),
		Bone(JOINT_LEFT_ELBOW, JOINT_LEFT_WRIST, vec3(1, 0, 0)),
		Bone(JOINT_LEFT_WRIST, JOINT_LEFT_HAND, vec3(1, 0, 0)),

		Bone(JOINT_WAIST, JOINT_LEFT_HIP, vec3(1, 0, 0)),
		Bone(JOINT_LEFT_HIP, JOINT_LEFT_KNEE, vec3(0, -1, 0)),
		Bone(JOINT_LEFT_KNEE, JOINT_LEFT_ANKLE, vec3(0, -1, 0)),

		Bone(JOINT_RIGHT_COLLAR, JOINT_RIGHT_SHOULDER, vec3(-1, 0, 0)),
		Bone(JOINT_RIGHT_SHOULDER, JOINT_RIGHT_ELBOW, vec3(-1, 0, 0)),
		Bone(JOINT_RIGHT_ELBOW, JOINT_RIGHT_WRIST, vec3(-1, 0, 0)),
		Bone(JOINT_RIGHT_WRIST, JOINT_RIGHT_HAND, vec3(-1, 0, 0)),

		Bone(JOINT_WAIST, JOINT_RIGHT_HIP, vec3(-1, 0, 0)),
		Bone(JOINT_RIGHT_HIP, JOINT_RIGHT_KNEE, vec3(0, -1, 0)),
		Bone(JOINT_RIGHT_KNEE, JOINT_RIGHT_ANKLE, vec3(0, -1, 0)),
	};

	for (int i = 0; i < bones.size(); i++) {
		auto j1 = joints[bones[i].from];
		auto j2 = joints[bones[i].to];

		if (j1.confidence < 0.15 || j2.confidence < 0.15) {
			continue;
		}
		// REAL -----
		/*
		glm::vec3 v1 = ofxnui::Tracker::fromVector3(j1.real);
		glm::vec3 v2 = ofxnui::Tracker::fromVector3(j2.real);

		ofVec3f v1map = ofVec3f(0, 0, 0);
		ofVec3f v2map = ofVec3f(0, 0, 0);
		v1map.x = ofMap(v1.x, -rgbFrameSize.x*0.5,rgbFrameSize.x*0.5, 0,APP_WIDTH);
		v1map.y = ofMap(v1.y, rgbFrameSize.y*0.5,-rgbFrameSize.y*0.5, 0,APP_HEIGT);
		v1map.z = 0;
		v2map.x = ofMap(v2.x, -rgbFrameSize.x*0.5,rgbFrameSize.x*0.5, 0,APP_WIDTH);
		v2map.y = ofMap(v2.y, rgbFrameSize.y*0.5,-rgbFrameSize.y*0.5, 0,APP_HEIGT);
		v2map.z = 0;
		ofVec4f v1warp = warper.fromScreenToWarpCoord(v1map.x,v1map.y,v1map.z);
		ofVec4f v2warp = warper.fromScreenToWarpCoord(v2map.x,v2map.y,v2map.z);

		// draw joint's
		ofSetColor(255, 255, 0); // yellow
		ofDrawLine(v1map,v2map);

		// draw joint's
		ofSetColor(0, 255, 255); // cian
		ofDrawLine(v1warp.x,v1warp.y,v2warp.x,v2warp.y);
		*/

		// PROJ -----
		glm::vec3 p1 = ofxnui::Tracker::fromVector3(j1.proj);
		glm::vec3 p2 = ofxnui::Tracker::fromVector3(j2.proj);

		ofVec3f p1map = ofVec3f(0, 0, 0);
		ofVec3f p2map = ofVec3f(0, 0, 0);
		p1map.x = ofMap(p1.x, 0.0,1.0, 0,APP_WIDTH);
		p1map.y = ofMap(p1.y, 0.0,1.0, 0,APP_HEIGT);
		p1map.z = 0;
		p2map.x = ofMap(p2.x, 0.0,1.0, 0,APP_WIDTH);
		p2map.y = ofMap(p2.y, 0.0,1.0, 0,APP_HEIGT);
		p2map.z = 0;
		p1map.x = APP_WIDTH-p1map.x; // TODO posr en un control aquest invertit per la pos de camera
		p2map.x = APP_WIDTH-p2map.x; // TODO posr en un control aquest invertit per la pos de camera
		ofVec4f p1warp = warper.fromScreenToWarpCoord(p1map.x,p1map.y,p1map.z);
		ofVec4f p2warp = warper.fromScreenToWarpCoord(p2map.x,p2map.y,p2map.z);

		/*
		// draw joint's
		ofSetColor(255, 0, 255); // magenta
		ofDrawLine(p1map,p2map);
		//ofDrawBitmapString("bone: " + ofToString(i) + " at " + ofToString(p1map.x) + ", " + ofToString(p1map.y), 20,300+i*20);
		*/

		// draw joint's
		ofSetColor(255, 128, 0); // taronja
		ofDrawLine(p1warp.x,p1warp.y,p2warp.x,p2warp.y);

	}

}

void ofApp::drawPointcloud() {
  ofPushMatrix();
  glPointSize(1);
  pc.draw();
  ofPopMatrix();
}
#endif

//--------------------------------------------------------------
void ofApp::exit(){
  warper.save("TrampoLima_CornerPin.xml");
  guia->saveSettings("TrampoLima_Deteccio.xml");
  delete guia;
  guib->saveSettings("TrampoLima_Botons.xml");
  delete guib;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  // H + F1 + a : help, aide, ayuda, ajuda
  // w : corner pin
  // J ajusta joc
  // G : grid -> per development (cal descomentar a baix)
  // D ajusta botons -> per development (cal descomentar a baix)

  if((key == 'h')||(key == 'H')||(key == 'a')||(key == 'A')||(key == OF_KEY_F1)){
      guia->setVisible(false);
      guib->setVisible(false);
      warper.deactivate();
      bshowImagesAndSkeleton = false;
      guih->toggleVisible();
  }
  else if((key == 'c')||(key == 'C')){
      guia->setVisible(false);
      guib->setVisible(false);
      guih->setVisible(false);
      bshowImagesAndSkeleton = false;
      warper.deactivate();
      bshowCamera = !bshowCamera;
  }
  else if((key == 'w')||(key == 'W')){
      guia->setVisible(false);
      guib->setVisible(false);
      guih->setVisible(false);
      bshowImagesAndSkeleton = false;
      warper.toggleActive();
  }
  else if((key == 'j')||(key == 'J')||(key == 'g')||(key == 'G')){
      guib->setVisible(false);
      guih->setVisible(false);
      warper.deactivate();
      guia->toggleVisible();
      bshowImagesAndSkeleton = !bshowImagesAndSkeleton;
  }
  else if((key == 'f')||(key == 'F')){
      ofToggleFullscreen();
  }
  else if((key == 'b')||(key == 'B')){
      toogleDrawInfoGrid();
      guia->setVisible(false);
      guih->setVisible(false);
      warper.deactivate();
      bshowImagesAndSkeleton = false;
      guib->toggleVisible();
  }
  else if((key == 'r')||(key == 'R')){
      // reset corners
      warper.setCorner(ofxGLWarper::TOP_LEFT, 0,0);
      warper.setCorner(ofxGLWarper::TOP_RIGHT, APP_WIDTH,0);
	  warper.setCorner(ofxGLWarper::TOP_RIGHT, APP_WIDTH, 0);
      warper.setCorner(ofxGLWarper::BOTTOM_RIGHT, APP_WIDTH, APP_HEIGT);
	  warper.setCorner(ofxGLWarper::BOTTOM_LEFT, 0, APP_HEIGT);
  }
  else if ((key == 'o') || (key == 'O')) {
	  // reset corners
	  warpMouseType = JOINT_LEFT_HAND;
  }
  else if ((key == 'p') || (key == 'P')) {
	  // reset corners
	  warpMouseType = JOINT_LEFT_ANKLE;
  }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
  ofVec4f v = warper.fromScreenToWarpCoord(x,y,0);
  warpMousePos.x = v.x;
  warpMousePos.y = v.y;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    int kind = e.widget->getKind();

    if(name == "reset corners"){
        // reset corners
        warper.setCorner(warper.TOP_LEFT, 0,0);
        warper.setCorner(warper.TOP_RIGHT, APP_WIDTH,0);
        warper.setCorner(warper.BOTTOM_RIGHT, APP_WIDTH,APP_HEIGT);
        warper.setCorner(warper.BOTTOM_LEFT, 0,APP_HEIGT);
    }
    else if(name == "load factory defaults"){
        // reset corners
        warper.setCorner(warper.TOP_LEFT, 0,0);
        warper.setCorner(warper.TOP_RIGHT, APP_WIDTH,0);
        warper.setCorner(warper.BOTTOM_RIGHT, APP_WIDTH,APP_HEIGT);
        warper.setCorner(warper.BOTTOM_LEFT, 0,APP_HEIGT);
        // parametres
        baixaHoTotAvall = 0; // adjustment fi up-down
        mouHoTotDretaEsq = 0; // adjustment fi left-right
    }
}

//--------------------------------------------------------------
//---PECES EMPTY -----------------------------------------------
//--------------------------------------------------------------
void ofApp::comprobarEstatsPecesEmpty(){

	for (int i = 0; i < aliments.size(); i++) {

		if (aliments[i].estatPeca == THE_END) { // seguent peca: tornem-hi
			setupPeca();
		}
	}
	if (end.estatPeca == THE_END) { // seguent peca: tornem-hi
		setupPeca();
	}
	if (plate.estatPeca == THE_END) { // seguent peca: tornem-hi
		setupPeca();
	}
	if (trash.estatPeca == THE_END) { // seguent peca: tornem-hi
		setupPeca();
	}
}

//--------------------------------------------------------------
void ofApp::setupPeca(){
     
	//while (myGrid.brectGridActiu[g] == false);
	for (int i = 0; i < aliments.size(); i++) {

		int g = aliments[i].pecaNumGraella;
		aliments[i].init(comptadorPeces, g, myGrid.returnPosicioOfPeca(g));
		comptadorPeces++;

		aliments[i].estatPeca = CANVIA_ESTAT;
		aliments[i].estatPeca = APAREIX;
		aliments[i].bpecaActiva = true;
	}

	int g = end.pecaNumGraella;
	end.init(comptadorPeces, g, myGrid.returnPosicioOfPeca(g));
	comptadorPeces++;

	end.estatPeca = CANVIA_ESTAT;
	end.estatPeca = APAREIX;
	end.bpecaActiva = true;

	g = plate.pecaNumGraella;
	plate.init(comptadorPeces, g, myGrid.returnPosicioOfPeca(g));
	comptadorPeces++;

	plate.estatPeca = CANVIA_ESTAT;
	plate.estatPeca = APAREIX;
	plate.bpecaActiva = true;

	g = trash.pecaNumGraella;
	trash.init(comptadorPeces, g, myGrid.returnPosicioOfPeca(g));
	comptadorPeces++;

	trash.estatPeca = CANVIA_ESTAT;
	trash.estatPeca = APAREIX;
	trash.bpecaActiva = true;
}



//--------------------------------------------------------------
//--- HELPERS --------------------------------------------------
//--------------------------------------------------------------
void ofApp::toogleDrawInfoGrid(){
    myGrid.toggleDrawInfoHelp();
	for (int i = 0; i < aliments.size(); i++) {
		aliments[i].toggleDrawInfoHelp();
	}
	end.toggleDrawInfoHelp();
	plate.toggleDrawInfoHelp();
	trash.toggleDrawInfoHelp();
    botoStart.toggleDrawInfoHelp();
    bdrawMouseWarped = !bdrawMouseWarped;
}

//--------------------------------------------------------------
string ofApp::pantallaToString(){
    if(pantallaJoc == START){
        return "START";
    }
    else if(pantallaJoc == PLAY){
        return "PLAY";
    }
    else if(pantallaJoc == END){
        return "END";
    }
    else{
        return "NO SE";
    }
}

//--------------------------------------------------------------
//--- PUNTUACIO ------------------------------------------------
//--------------------------------------------------------------
void ofApp::actualitzaPuntsEmpty(int & e){
    //singleton->setPuntuacioJugador(singleton->getPuntuacioJugador() + e);
}

//--------------------------------------------------------------
void ofApp::drawTemps(){
    string s = ofToString(jocMinutsTimerMinuts,2,'0') + ":" + ofToString(jocMinutsTimerSegons,2,'0');
    ofRectangle rs;
        rs = saltingTypo.getStringBoundingBox(s,0,0);
        ofPushMatrix();
    ofTranslate(APP_WIDTH_MEITAT-rs.width*2,35);
    if(jocMinutsTimerSegonsLeft < 60){
        ofSetColor(255,0,0,255); // temps en vermell
    }
    else{
        ofSetColor(255); // temps en blanc
    }
    saltingTypo.drawString(s,0,0);
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawPuntuacio(){
    string s = ofToString(money);
    ofRectangle rs;
    rs = saltingTypo.getStringBoundingBox(s,0,0);
    ofPushMatrix();
    ofTranslate(APP_WIDTH_MEITAT - rs.width*0.5, 35);
    ofSetColor(black);
    saltingTypo.drawString(s, 0, 0);
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawEnd(){
	bg_end.draw(0, 0);
	string s1 = "GREAT JOB!!";
	string s2 = ofToString(money) + " MONEY";
	ofRectangle rs1;
	ofRectangle rs2;
		rs1 = saltingTypo.getStringBoundingBox(s1, 0, 0);
		rs1 = saltingTypo.getStringBoundingBox(s2, 0, 0);
        ofPushMatrix();
	ofTranslate(APP_WIDTH_MEITAT - rs1.width*0.5, APP_HEIGT_MEITAT - rs1.height*0.5);
    ofSetColor(black);
	saltingTypo.drawString(s1, 0, - APP_HEIGT_MEITAT * 0.75);
    ofPopMatrix();
	ofTranslate(APP_WIDTH_MEITAT - rs2.width*0.5, APP_HEIGT_MEITAT - rs2.height*0.5);
	ofSetColor(black);
	saltingTypo.drawString(s2, 0, APP_HEIGT_MEITAT * 0.80);
	ofPopMatrix();
}

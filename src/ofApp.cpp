
//
//  Starter file for Project 3 - Skeleton Builder
//
//  This file includes functionality that supports selection and translate/rotation
//  of scene objects using the mouse.
//
//  Modifer keys for rotatation are x, y and z keys (for each axis of rotation)
//
//  (c) Kevin M. Smith  - 24 September 2018
//

#include <iostream>
#include <fstream>
#include "ofApp.h"

static int jointIndex = 0;
string jointFile = "joints.txt";

//--------------------------------------------------------------
//
void ofApp::setup() {

	ofSetBackgroundColor(ofColor::black);
	ofEnableDepthTest();
	mainCam.setDistance(15);
	mainCam.setNearClip(.1);
	
	sideCam.setPosition(40, 0, 0);
	sideCam.lookAt(glm::vec3(0, 0, 0));
	topCam.setNearClip(.1);
	topCam.setPosition(0, 16, 0);
	topCam.lookAt(glm::vec3(0, 0, 0));
	ofSetSmoothLighting(true);


	// setup one point light
	//
	light1.enable();
	light1.setPosition(5, 5, 0);
	light1.setDiffuseColor(ofColor(255.f, 255.f, 255.f));
	light1.setSpecularColor(ofColor(255.f, 255.f, 255.f));

	theCam = &mainCam;

	//  create a scene consisting of a ground plane with 2x2 blocks
	//  arranged in semi-random positions, scales and rotations
	//
	// ground plane
	//
	scene.push_back(new Plane(glm::vec3(0, -2, 0), glm::vec3(0, 1, 0)));   

	/*
	Sphere* sun = new Sphere(glm::vec3(0, 0, 0), 3.0, ofFloatColor::darkGoldenRod);
	Sphere* earth = new Sphere(glm::vec3(5, 1, 0), 1.0, ofColor::cadetBlue);
	Sphere* moon = new Sphere(glm::vec3(2, 0, 0), 0.25);
	sun->addChild(earth);
	earth->addChild(moon);

	//scene.push_back(sun);
	scene.push_back(earth);
	scene.push_back(moon);
	*/
	
	//scene.push_back(new Cone(glm::vec3(3, 0, 0), glm::vec3(45, 0, 90), glm::vec3(1, 5, 1)));
	//scene.push_back(new Cone(glm::vec3(3, 0, 0), glm::vec3(45, 0, 75), glm::vec3(1, 5, 1)));
	//scene.push_back(new Cone(glm::vec3(3, 0, 0), glm::vec3(45, 0, 60), glm::vec3(1, 5, 1)));
	//scene.push_back(new Cone(glm::vec3(3, 0, 0), glm::vec3(45, 0, 45), glm::vec3(1, 5, 1)));
	//scene.push_back(new Cone(glm::vec3(3, 0, 0), glm::vec3(45, 0, 30), glm::vec3(1, 5, 1)));
	//scene.push_back(new Cone(glm::vec3(3, 0, 0), glm::vec3(45, 0, 15), glm::vec3(1, 5, 1)));
	//scene.push_back(new Cone(glm::vec3(1, 2, 0), glm::vec3(0, 0, 0), glm::vec3(1, 2, 1)));
	//scene.push_back(new Sphere(glm::vec3(1, 0, 0), 1.0));
}
 
//--------------------------------------------------------------
void ofApp::update(){
	//scene[1]->rotation.y++;
	//scene[2]->rotation.y++;
	//scene[2]->scale.x += 0.01;*/
}

//--------------------------------------------------------------
void ofApp::draw(){

	theCam->begin();
	ofNoFill();
	drawAxis();
	ofEnableLighting();

	//  draw the objects in scene
	//
	material.begin();
	ofFill();
	for (int i = 0; i < scene.size(); i++) {
		if (objSelected() && scene[i] == selected[0])
			ofSetColor(ofColor::white);
		else ofSetColor(scene[i]->diffuseColor);
		scene[i]->draw();
	}

	material.end();
	ofDisableLighting();
	theCam->end();
}

// 
// Draw an XYZ axis in RGB at transform
//
void ofApp::drawAxis(glm::mat4 m, float len) {

	ofSetLineWidth(1.0);

	// X Axis
	ofSetColor(ofColor(255, 0, 0));
	ofDrawLine(glm::vec3(m*glm::vec4(0, 0, 0, 1)), glm::vec3(m*glm::vec4(len, 0, 0, 1)));


	// Y Axis
	ofSetColor(ofColor(0, 255, 0));
	ofDrawLine(glm::vec3(m*glm::vec4(0, 0, 0, 1)), glm::vec3(m*glm::vec4(0, len, 0, 1)));

	// Z Axis
	ofSetColor(ofColor(0, 0, 255));
	ofDrawLine(glm::vec3(m*glm::vec4(0, 0, 0, 1)), glm::vec3(m*glm::vec4(0, 0, len, 1)));
}

// print C++ code for obj tranformation channels. (for debugging);
//
void ofApp::printChannels(SceneObject *obj) {
	cout << "position = glm::vec3(" << obj->position.x << "," << obj->position.y << "," << obj->position.z << ");" << endl;
	cout << "rotation = glm::vec3(" << obj->rotation.x << "," << obj->rotation.y << "," << obj->rotation.z << ");" << endl;
	cout << "scale = glm::vec3(" << obj->scale.x << "," << obj->scale.y << "," << obj->scale.z << ");" << endl;
}

void ofApp::removeObject(SceneObject* obj) {

	// remove from scene list;
	//
	for (int i = 0; i < scene.size(); i++) {
		if (scene[i] == obj) {
			scene.erase(scene.begin() + i);
			break;
		}
	}

	// delete object;  sceneObject destructor will handle cleaning up parent and child lists.
	//
	delete obj;
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){

	switch (key) {
	case OF_KEY_ALT:
		bAltKeyDown = false;
		mainCam.disableMouseInput();
		break;
	case 'x':
		bRotateX = false;
		break;
	case 'y':
		bRotateY = false;
		break;
	case 'z':
		bRotateZ = false;
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	case 'C':
	case 'c':
		if (mainCam.getMouseInputEnabled()) mainCam.disableMouseInput();
		else mainCam.enableMouseInput();
		break;
	case 'F':
	case 'b':
		break;
	case 'f':
		ofToggleFullscreen();
		break;
	case 'h':
		bHide = !bHide;
		break;
	case 'i':
		break;
	case 'j':
	{
		// Create a new joint at the origin
		Joint* j = new Joint("joint" + to_string(jointIndex++), glm::vec3(0, 0, 0), 1);
		cout << "New joint: " << j->name << endl << flush;
		if (objSelected()) // if another object was selected when creating this new joint
		{
			// Check if the selected object is also a joint
			Joint* parent = dynamic_cast<Joint*>(selected[0]);
			if (parent != nullptr)
			{
				cout << "parent: " << parent->name << endl << flush;
				parent->addChild(j); // connect new joint to selected joint
				j->position[1] = -3; // move child joint 3 units below parent
			}
		}
		scene.push_back(j);	// add new joint to list of scenes
		break;
	}
	case 'n':
		break;
	case 'p':
		if (objSelected()) printChannels(selected[0]);
		break;
	case 'R':
	case 'r':
	{
		readFromFile();
		break;
	}
	case 'S':
	case 's':
	{
		saveToFile();
		break;
	}
	case 'x':
		bRotateX = true;
		break;
	case 'y':
		bRotateY = true;
		break;
	case 'z':
		bRotateZ = true;
		break;
	case OF_KEY_F1: 
		theCam = &mainCam;
		break;
	case OF_KEY_F2:
		theCam = &sideCam;
		break;
	case OF_KEY_F3:
		theCam = &topCam;
		break;
	case OF_KEY_ALT:
		bAltKeyDown = true;
		if (!mainCam.getMouseInputEnabled()) mainCam.enableMouseInput();
		break;
	case OF_KEY_DEL:
	case OF_KEY_BACKSPACE:
		if (objSelected()) {
			removeObject(selected[0]);
			clearSelectionList();
		}
		break;

	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

	if (objSelected() && bDrag) {
		glm::vec3 point; 
		mouseToDragPlane(x, y, point);
		if (bRotateX) {
			selected[0]->rotation += glm::vec3((point.x - lastPoint.x) * 20.0, 0, 0);
		}
		else if (bRotateY) {
			selected[0]->rotation += glm::vec3(0, (point.x - lastPoint.x) * 20.0, 0);
		}
		else if (bRotateZ) {
			selected[0]->rotation += glm::vec3(0, 0, (point.x - lastPoint.x) * 20.0);
		}
		else {
			selected[0]->position += (point - lastPoint);
			if (selected[0]->parent)
			{
				// rotate to face parent
				//glm::mat4 rotate = selected[0]->rotateToVector(glm::vec3(0, 1, 0), selected[0]->parent->position);
				//glm::mat4 rotate = selected[0]->getRotateMatrix();
				//glm::vec3 rotation = rotate * glm::vec4(glm::vec3(0, 1, 0), 1.0);
				//selected[0]->rotation = rotation * 180.0 / PI;
				//cout << "Rotate:" << endl << rotate << endl << "pos: " << selected[0]->position << "; rot: " << rotation << endl;
			}
		}
		lastPoint = point;
	}

}

//  This projects the mouse point in screen space (x, y) to a 3D point on a plane
//  normal to the view axis of the camera passing through the point of the selected object.
//  If no object selected, the plane passing through the world origin is used.
//
bool ofApp::mouseToDragPlane(int x, int y, glm::vec3 &point) {
	glm::vec3 p = theCam->screenToWorld(glm::vec3(x, y, 0));
	glm::vec3 d = p - theCam->getPosition();
	glm::vec3 dn = glm::normalize(d);

	float dist;
	glm::vec3 pos;
	if (objSelected()) {
		pos = selected[0]->position;
	}
	else pos = glm::vec3(0, 0, 0);
	if (glm::intersectRayPlane(p, dn, pos, glm::normalize(theCam->getZAxis()), dist)) {
		point = p + dn * dist;
		return true;
	}
	return false;
}

//--------------------------------------------------------------
//
// Provides functionality of single selection and if something is already selected,
// sets up state for translation/rotation of object using mouse.
//
void ofApp::mousePressed(int x, int y, int button){

	// if we are moving the camera around, don't allow selection
	//
	if (mainCam.getMouseInputEnabled()) return;

	// clear selection list
	//
	selected.clear();

	//
	// test if something selected
	//
	vector<SceneObject *> hits;

	glm::vec3 p = theCam->screenToWorld(glm::vec3(x, y, 0));
	glm::vec3 d = p - theCam->getPosition();
	glm::vec3 dn = glm::normalize(d);

	// check for selection of scene objects
	//
	for (int i = 0; i < scene.size(); i++) {
		
		glm::vec3 point, norm;
		
		//  We hit an object
		//
		if (scene[i]->isSelectable && scene[i]->intersect(Ray(p, dn), point, norm)) {
			hits.push_back(scene[i]);
		}
	}


	// if we selected more than one, pick nearest
	//
	SceneObject *selectedObj = NULL;
	if (hits.size() > 0) {
		selectedObj = hits[0];
		float nearestDist = std::numeric_limits<float>::infinity();
		for (int n = 0; n < hits.size(); n++) {
			float dist = glm::length(hits[n]->position - theCam->getPosition());
			if (dist < nearestDist) {
				nearestDist = dist;
				selectedObj = hits[n];
			}	
		}
	}
	if (selectedObj) {
		selected.push_back(selectedObj);
		bDrag = true;
		mouseToDragPlane(x, y, lastPoint);
	}
	else {
		selected.clear();
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	bDrag = false;

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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

void ofApp::saveToFile()
{
	ofstream jout(jointFile); // open joint file for writing
	if (!jout.is_open())
	{
		cout << "Cannot open " << jointFile << endl;
		return;
	}
	// for each scene object, check if it is a joint
	for (SceneObject* obj : scene)
	{
		Joint* j = dynamic_cast<Joint*>(obj);
		if (j == nullptr) continue;	// skip because this object is not a Joint
		// write a line to the joint file in the format:
		//   create -joint "joint name" -rotate "<x, y, z>" -translate "<x, y, z>" -parent "name"
		jout << "create -joint " << j->name
			 << " -rotate <"     << j->rotation[0] << ", " << j->rotation[1] << ", " << j->rotation[2]
			 << "> -translate <" << j->position[0] << ", " << j->position[1] << ", " << j->position[2]
			 << ">";
		if (j->parent != nullptr)
			jout << " -parent " << j->parent->name;
		jout << ";" << endl;
	}
	jout.close();	// close joint file
}

void ofApp::readFromFile()
{
	vector<Joint*> joints; // local copy of joints for finding parents
	ifstream jin(jointFile); // open joint file for reading
	if (!jin.is_open())
	{
		cout << "Cannot open " << jointFile << endl;
		return;
	}
	cout << "read " << jointFile << endl;
	string line; // buffer to store each line read from the joint file
	while (getline(jin, line)) // read line from the joint file
	{
		cout << line << endl;
		stringstream ss(line); // use string stream to stream out the line
		string tmp;			   // temp to skip words
		char ctmp;			   // temp to skip characters
		string jointName;	   // store name of the joint
		double r0, r1, r2;	   // store rotation vector
		double p0, p1, p2;	   // store translation vector
		string parentName;	   // store the parent name

		ss >> tmp; // "create"
		ss >> tmp; // "-joint"
		ss >> jointName;
		ss >> tmp; // "-rotate"
		ss >> ctmp; // "<"
		ss >> r0;
		ss >> ctmp; // ","
		ss >> r1;
		ss >> ctmp; // ","
		ss >> r2;
		ss >> ctmp; // ">"
		ss >> tmp; // "-translate"
		ss >> ctmp; // "<"
		ss >> p0;
		ss >> ctmp; // ","
		ss >> p1;
		ss >> ctmp; // ","
		ss >> p2;
		ss >> ctmp; // ">"
		ss >> tmp; // "-parent"
		if (tmp == "-parent")
		{
			ss >> parentName;
			// discard trailing ";"
			parentName.erase(parentName.size() - 1);
		}

		// create a new joint from the information read from the file
		Joint* j = new Joint(jointName, glm::vec3(p0, p1, p2), 1);
		j->rotation = glm::vec3(r0, r1, r2);
		// if the joint has a parent
		if (tmp == "-parent")
		{
			// Parent joint name is specified, so search through previously read joints
			for (Joint* jp : joints)
			{
				// if joint name matches the parent name
				if (jp->name == parentName)
				{
					jp->addChild(j); // add this new joint to parent joint's child list
					break; // found parent joint, so stop looking
				}
			}
		}
		scene.push_back(j); // add new joint to the scene list
		joints.push_back(j);	// add new joint to the joints list for finding parents
	}
	jin.close();	// close file
}
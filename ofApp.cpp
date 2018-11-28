#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetCircleResolution(30);
	ofSetLineWidth(2);
	//ofNoFill();

	this->size = 32;
	ofTrueTypeFontSettings font_settings("fonts/EmojiSymbols-Regular.ttf", this->size);
	font_settings.antialiased = true;
	font_settings.addRanges(ofAlphabet::Emoji);
	this->font.load(font_settings);

	this->box2d.init();
	this->box2d.setGravity(0, 5);
	this->box2d.createBounds();
	this->box2d.setFPS(60);
	this->box2d.registerGrabbing();


}

//--------------------------------------------------------------
void ofApp::update() {

	this->left = false;
	this->right = false;

	if (ofRandom(100) > 85) {

		if (ofRandom(2) > 1) {

			this->left = true;
		}
		else {

			this->right = true;
		}

		float radius = this->size * 0.5;
		auto circle = make_shared<ofxBox2dCircle>();
		circle->setPhysics(1.0, 0.63, 0.1);
		circle->setup(this->box2d.getWorld(), this->left ? ofGetWidth() * 0.25 : ofGetWidth() * 0.75, ofGetHeight() * 0.5 + 100, radius);
		circle->addForce(ofVec2f(ofRandom(-10, 10), ofRandom(-10, 10)), 150.0);
		this->circles.push_back(circle);

		this->note_types.push_back(ofRandom(2));

		ofColor color;
		color.setHsb(ofRandom(255), 255, 230);
		this->colors.push_back(color);

		this->lifes.push_back(255);
	}

	for (int i = 0; i < this->circles.size(); i++) {

		this->lifes[i]--;
		if (this->lifes[i] < 0) {

			this->circles[i].get()->destroy();
			this->circles.erase(this->circles.begin() + i);
			this->note_types.erase(this->note_types.begin() + i);
			this->colors.erase(this->colors.begin() + i);
			this->lifes.erase(this->lifes.begin() + i);
		}
	}

	this->box2d.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	// Draw Speaker Left
	ofPushMatrix();
	ofTranslate(ofGetWidth() * 0.25, ofGetHeight() * 0.5);

	ofSetColor(39);
	ofDrawRectangle(-150, -300, 300, 600);
	ofSetColor(139);
	ofDrawCircle(50, -100, 50);
	this->left ? ofDrawCircle(0, 100, 100) : ofDrawCircle(0, 100, 110);

	ofPopMatrix();

	// Draw Speaker Right
	ofPushMatrix();
	ofTranslate(ofGetWidth() * 0.75, ofGetHeight() * 0.5);

	ofSetColor(39);
	ofDrawRectangle(-150, -300, 300, 600);
	ofSetColor(139);
	ofDrawCircle(-50, -100, 50);
	this->right ? ofDrawCircle(0, 100, 100) : ofDrawCircle(0, 100, 110);

	ofPopMatrix();

	// Draw Note
	for (int i = 0; i < this->circles.size(); i++) {

		ofPushMatrix();
		ofTranslate(this->circles[i]->getPosition().x, this->circles[i]->getPosition().y);
		ofRotate(this->circles[i]->getRotation());

		ofSetColor(this->colors[i], this->lifes[i] + 32);
		this->note_types[i] == 0 ? this->font.drawString(u8"🎵", this->circles[i]->getRadius() * -1, this->circles[i]->getRadius()) : this->font.drawString(u8"🎶", this->circles[i]->getRadius() * -1, this->circles[i]->getRadius());

		ofPopMatrix();
	}

}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
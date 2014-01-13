#include "plane.h"

void Plane::init(int i){
    index = i;
    keyVals.push_back(ofPoint(0,0));
    keyVals.push_back(ofPoint(1,0));
    keyVals.push_back(ofPoint(0,1));
    keyVals.push_back(ofPoint(1,1));

    group = false;
    drawBox = false;
    xRes = 10;
    yRes = 10;
    pointIndex = -1;
	value = 1;
    width = 1023;
    height = 768;
    
}


void Plane::setup(){
    int w = width;
    int h = height;
    
    int x = index*w;
    int y = 0;
    
    value = 1;
    
    position.push_back(x);
    position.push_back(y);
    
    mesh.clear();
    mesh = ofMesh::plane(w, h, xRes, yRes, OF_PRIMITIVE_TRIANGLES);
    
    vector<ofVec3f> v = mesh.getVertices();
    
    for (int i=0; i<v.size(); i++) {
        
        mesh.setVertex(i, ofVec3f(
                                  v[i].x + w/2 + x,
                                  v[i].y + h/2 + y,
                                  v[i].z    ));
        
        mesh.setTexCoord(i, ofVec2f(
                                    v[i].x + w/2,
                                    v[i].y + h/2    ));
    }

    mesh.load("models/plane-mesh-" + ofToString(index+1) + ".ply");
    cout << "plane-mesh-" + ofToString(index+1) + ".ply"  << endl;

    for (int i=0; i<v.size(); i++) {
        gridVerts.push_back(ofVec3f(0,0,0));
        orgVerts.push_back(mesh.getVertex(i));
    }
    
    ofPoint tl(keyVals[0].x*w+x, keyVals[0].y*h+y);
    ofPoint tr(keyVals[1].x*w+x, keyVals[1].y*h+y);
    ofPoint bl(keyVals[2].x*w+x, keyVals[2].y*h+y);
    ofPoint br(keyVals[3].x*w+x, keyVals[3].y*h+y);
    
    keystone.setAnchorSize(w/2, h/2);
    keystone.setSourceRect(ofRectangle(x,y,w,h));
    keystone.setTopLeftCornerPosition(tl);
    keystone.setTopRightCornerPosition(tr);
    keystone.setBottomLeftCornerPosition(bl);
    keystone.setBottomRightCornerPosition(br);
    keystone.setup();

    for (int i=0; i<v.size(); i++) {
       v[i] = keystone.getMatrix().preMult( gridVerts[i] + orgVerts[i]  );
       mesh.setVertex(i, v[i]);
    }
 
  
}

void Plane::resetKeystone(){
    keystone.reset();
}

void Plane::resetGrid(){
    int w = width;
    int h = height;
    int x = index*w;
    int y = 0;
    
    position[0] = x;
    position[1] = y;
    
    mesh.clear();
    mesh = ofMesh::plane(w, h, xRes, yRes, OF_PRIMITIVE_TRIANGLES);
    
    vector<ofVec3f> v = mesh.getVertices();
    
    for (int i=0; i<v.size(); i++) {
        mesh.setVertex(i, ofVec3f(
                                  v[i].x + w/2 + x,
                                  v[i].y + h/2 + y,
                                  v[i].z    ));
        
        mesh.setTexCoord(i, ofVec2f(
                                    v[i].x + w/2,
                                    v[i].y + h/2    ));
    }
    
    for (int i=0; i<v.size(); i++) {
        gridVerts[i] = ofVec3f(0,0,0);
        orgVerts[i] = mesh.getVertex(i);
    }
 
}


void Plane::draw(){
    vector<ofVec3f> v = mesh.getVertices();
    
    for (int i=0; i<v.size(); i++) {
        v[i] = keystone.getMatrix().preMult( gridVerts[i] + orgVerts[i] );
        //v[i] = gridVerts[i] + orgVerts[i];
        mesh.setVertex(i, v[i]);
    }
    mesh.draw();
}


void Plane::drawConfig(){
    if (drawBox) {
        ofNoFill();
        glLineWidth(2);
        ofSetHexColor(0xFFFFFF);
        ofRect(boxOrigin.x, boxOrigin.y, boxUpdate.x-boxOrigin.x, boxUpdate.y-boxOrigin.y);
    }
        
    vector<ofVec3f> v = mesh.getVertices();
    
    float rad = 4;
    for (int i=0; i<v.size(); i++) {
        if(sel[i])
            ofSetHexColor(0xFFF000);
        else
            ofSetHexColor(0xFFFFFF);
        ofDrawSphere(v[i].x, v[i].y, v[i].z, rad );
    }
}

//--------------------------------------------------------------
void Plane::keyPressed(int key){
    if (key == OF_KEY_SHIFT) {
        shift = true;
    }
    
    else if (key == OF_KEY_UP) {
        
        for (int i=0; i<sel.size(); i++) {
            if (sel[i]) {
                float x = mesh.getVertex(i).x;
                float y = mesh.getVertex(i).y;
                float z = mesh.getVertex(i).z;
                y--;
                mesh.setVertex( i, ofVec3f(x,y,z) );
            }
        }
    }
    else if (key == OF_KEY_DOWN) {
        
        for (int i=0; i<sel.size(); i++) {
            if (sel[i]) {
                float x = mesh.getVertex(i).x;
                float y = mesh.getVertex(i).y;
                float z = mesh.getVertex(i).z;
                y++;
                mesh.setVertex( i, ofVec3f(x,y,z) );
            }
        }
    }
    else if (key == OF_KEY_LEFT) {
        
        for (int i=0; i<sel.size(); i++) {
            if (sel[i]) {
                float x = mesh.getVertex(i).x;
                float y = mesh.getVertex(i).y;
                float z = mesh.getVertex(i).z;
                x--;
                mesh.setVertex( i, ofVec3f(x,y,z) );
            }
        }
    }
    else if (key == OF_KEY_RIGHT) {
        
        for (int i=0; i<sel.size(); i++) {
            if (sel[i]) {
                float x = mesh.getVertex(i).x;
                float y = mesh.getVertex(i).y;
                float z = mesh.getVertex(i).z;
                x++;
                mesh.setVertex( i, ofVec3f(x,y,z) );
            }
        }
    }
}


//--------------------------------------------------------------
void Plane::keyReleased(int key){
    if (key == OF_KEY_SHIFT) {
        shift = false;
    }
}


//--------------------------------------------------------------
void Plane::onMouseDragged(ofMouseEventArgs& mouseArgs){
        
    ofPoint mousePoint(mouseArgs.x, mouseArgs.y);
    
    if (keystoneActive) {
        keystone.onMouseDragged(mouseArgs);
    }
    else if (gridActive) {
        if (group) {
            drawBox = true;
            boxUpdate = ofPoint(mousePoint.x, mousePoint.y);
            return;
        }
        
        if (pointIndex == -1)
            return;
        
        for (int i=0; i<sel.size(); i++) {
            if (sel[i]) {
                ofPoint newPoint = ofPoint(gridVerts[i].x, gridVerts[i].y) - ((lastM - mousePoint) * value);
                gridVerts[i] = ofVec3f(newPoint.x, newPoint.y, newPoint.z);
            }
        }
    }
    
    lastM = mousePoint;    
}

//--------------------------------------------------------------
void Plane::onMousePressed(ofMouseEventArgs& mouseArgs){

    int x = mouseArgs.x;
    int y = mouseArgs.y;
    
    if (keystoneActive) {
        keystone.onMousePressed(mouseArgs);
    }
    else if (gridActive) {
        vector<ofVec3f> v = mesh.getVertices();
        float rad = 20;

        for (int i=0; i<v.size(); i++) {
            float distance = ofDist(v[i].x, v[i].y, x, y);
            
            if (distance < rad) {
                pointIndex = i;
                
                if (!sel[i]) {
                    
                    if (shift) {
                        sel[i] = true;
                    }
                    else {
                        sel.clear();
                        sel[i] = true;
                    }
                }
                group = false;
                break;
            }
            else {
                pointIndex = -1;
                group = true;
                drawBox = true;
                boxOrigin = ofPoint(x,y);
                boxUpdate = boxOrigin;
            }
        }
    }

    lastM = ofPoint(x,y);
}

//--------------------------------------------------------------
void Plane::onMouseReleased(ofMouseEventArgs& mouseArgs){
    
    int x = mouseArgs.x;
    int y = mouseArgs.y;
    
    if (gridActive && group) {
        vector<ofVec3f> v = mesh.getVertices();
        
        float rad = 20;
        
        ofRectangle rect;
        rect = ofRectangle(boxOrigin.x, boxOrigin.y, boxUpdate.x-boxOrigin.x, boxUpdate.y-boxOrigin.y);
        
        
        for (int i=0; i<v.size(); i++) {
            
            if ( (v[i].x) + rad    >= rect.getTopLeft().x
                && (v[i].x) - rad   <= rect.getBottomRight().x) {
                
                if ( (v[i].y) + rad >= rect.getTopLeft().y
                    && (v[i].y) - rad    <= rect.getBottomRight().y) {
                    
                    sel[i] = true;
                }
                else  {
                    sel[i] = false;
                }
            }
            else  {
                sel[i] = false;
            }
        }
        
        drawBox = false;
        group = false;
    }
}


void Plane::load(ofXml &xml) {
    mesh.load("plane-mesh-" + ofToString(index+1) + ".ply");
    string xmlPrefix = "projectors/projector[";
    
    string pre = xmlPrefix + ofToString(index);
    
    if (xml.exists(pre + "][@keystone]")) {
        string str = xml.getAttribute(pre + "][@keystone]");
        keyVals[0].x = ofToFloat(ofSplitString(str, ",")[0]);
        keyVals[0].y = ofToFloat(ofSplitString(str, ",")[1]);
        keyVals[1].x = ofToFloat(ofSplitString(str, ",")[2]);
        keyVals[1].y = ofToFloat(ofSplitString(str, ",")[3]);
        keyVals[2].x = ofToFloat(ofSplitString(str, ",")[4]);
        keyVals[2].y = ofToFloat(ofSplitString(str, ",")[5]);
        keyVals[3].x = ofToFloat(ofSplitString(str, ",")[6]);
        keyVals[3].y = ofToFloat(ofSplitString(str, ",")[7]);
    }
    setup();
}


void Plane::save(ofXml &xml) {
    
    ofMesh m = mesh;
    vector<ofVec3f> v = m.getVertices();
    for (int i=0; i<v.size(); i++) {
        v[i] = gridVerts[i] + orgVerts[i];
        m.setVertex(i, v[i]);
    }
    m.save("plane_mesh_" + ofToString(index) + ".ply");
    cout << keyVals[0].x  << endl;

    string xmlPrefix = "projectors/projector[";

    string pre = xmlPrefix + ofToString(index);
    
    int w = 1024;
    int h = 768;
    int x = index*w;
    int y = 0;
    
    xml.setAttribute(pre + "][@keystone]",
                     
                     ofToString((keystone.dstPoints[0].x-x)/w) +  "," +
                     ofToString((keystone.dstPoints[0].y-y)/h) +  "," +
                     ofToString((keystone.dstPoints[1].x-x)/w) +  "," +
                     ofToString((keystone.dstPoints[1].y-y)/h) +  "," +
                     ofToString((keystone.dstPoints[3].x-x)/w) +  "," +
                     ofToString((keystone.dstPoints[3].y-y)/h) +  "," +
                     ofToString((keystone.dstPoints[2].x-x)/w) +  "," +
                     ofToString((keystone.dstPoints[2].y-y)/h) );
 
}



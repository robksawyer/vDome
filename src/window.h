#include "ofMain.h"
namespace vd {

class Window {

public:
    Window();
    void setup();

    void loadXML(ofXml &xml);
    void saveXML(ofXml &xml);

    ofVec2f getPosition();
    void setPosition(int x, int y);

    ofVec2f getDimensions();
    void setDimensions(int w, int h);

private:
    ofVec2f position;
    ofVec2f dimensions;
	bool fullscreen;
};

}

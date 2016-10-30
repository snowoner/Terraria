#ifndef _ANIMATION_KEYFRAMES
#define _ANIMATION_KEYFRAMES


#include <vector>


using namespace std;


// AnimKeyframes contains all information related to a single animation.
// These are the animation speed measured by millisecsPerKeyframe
// and texture coordinates for all keyframes.


struct AnimKeyframes
{
	float millisecsPerKeyframe;
	float scaleX;
	vector<glm::vec2> keyframeDispl;
	vector<glm::vec2> keyframeRot;
	vector<glm::vec2> keyframePosDisp;

};


#endif // _ANIMATION_KEYFRAMES



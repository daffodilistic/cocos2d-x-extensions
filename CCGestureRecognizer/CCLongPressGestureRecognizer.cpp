/****************************************************************************
Copyright (c) 2013 Artavazd Barseghyan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCLongPressGestureRecognizer.h"

USING_NS_CC;

bool CCLongPressGestureRecognizer::init()
{
    isRecognizing = false;
    currTouch = NULL;
    currEvent = NULL;
    
    setMinimumPressDuration(kLongPressMinDuration);
    
    return true;
}

CCLongPressGestureRecognizer::~CCLongPressGestureRecognizer()
{
    
}

void CCLongPressGestureRecognizer::timerDidEnd(float dt)
{
    // check if the current touch is near the original touch
    float distance = currLocation.getDistance(origLocation);
    if (distance > kLongPressDistanceTolerance) {
        stopGestureRecognition();
        return;
    }
    
    CCGesture * longPress = CCGesture::create();
    longPress->location = currLocation;
    longPress->cancelPropagation = cancelsTouchesInView;
    
    gestureRecognized(longPress);
    if (longPress->cancelPropagation) stopTouchesPropagation(currEvent); //cancel touch over other views
    
    stopGestureRecognition();
}

bool CCLongPressGestureRecognizer::onTouchBegan(Touch * pTouch, Event * pEvent)
{
    if (isRecognizing) {
        stopGestureRecognition();
        return false;
    }
    
    origLocation = currLocation = pTouch->getLocation();
    if (!isPositionBetweenBounds(origLocation)) return false;
    
    currEvent = pEvent;
    currTouch = pTouch;
    
    schedule(schedule_selector(CCLongPressGestureRecognizer::timerDidEnd), minimumPressDuration);
    
    isRecognizing = true;
    return true;
}

void CCLongPressGestureRecognizer::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    currLocation = pTouch->getLocation();
}

void CCLongPressGestureRecognizer::onTouchEnded(Touch * pTouch, Event * pEvent)
{
    stopGestureRecognition();
}

void CCLongPressGestureRecognizer::stopGestureRecognition()
{
    if (!isRecognizing) return;
    
    currTouch = NULL;
    currEvent = NULL;
    unschedule(schedule_selector(CCLongPressGestureRecognizer::timerDidEnd));
    isRecognizing = false;
}
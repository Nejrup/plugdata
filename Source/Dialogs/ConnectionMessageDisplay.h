#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "Constants.h"
#include "LookAndFeel.h"
#include "../Connection.h"
#include "../PluginEditor.h"

class ConnectionMessageDisplay 
    : public Component
    , public MultiTimer {
public:
    ConnectionMessageDisplay(PluginEditor* editor)
        : editor(editor)
    {
        setSize(36,36);
        startTimer(RepaintTimer, 1000/60);
        setVisible(false);
        // needed to stop the component from gaining mouse focus
        setInterceptsMouseClicks(false, false);
    }

    ~ConnectionMessageDisplay()
    {
    }

    /** Set the current connection show message display overlay, to clear give it a nullptr
    */
    void setConnection(Connection* connection)
    {
        activeConnection = SafePointer<Connection>(connection);
        if (activeConnection.getComponent()) {
            auto canvasConnectionPath = activeConnection->toDraw;
            setCentrePosition(getParentComponent()->getLocalPoint(nullptr, connection->mouseHoverPos).translated(0, -(getHeight() * 0.5)));
            startTimer(MouseHoverDelay, mouseDelay);
            stopTimer(MouseHoverExitDelay);
            updateTextString(true);
        }
        else {
            setVisible(false);
            stopTimer(MouseHoverDelay);
            // to copy tooltip behaviour, any successful interaction will cause the next interaction to have no delay
            mouseDelay = 0;
            startTimer(MouseHoverExitDelay, 500);
        }
    }

private:
    void updateTextString(bool isHoverEntered = false)
    {
        textString = activeConnection->getMessageTooltip();

        if (textString.isEmpty())
            textString = String("no message yet");

        Font textFont(Fonts::getCurrentFont());
        auto stringWidth = textFont.withHeight(14).getStringWidth(textString);
        // add margin and padding
        stringWidth += (8 * 2) + (4 + 4);

        // only make the size wider, to fit changing values
        if (stringWidth > getWidth() || isHoverEntered)
            setSize(stringWidth, 36);

        repaint();
    }

    void timerCallback(int ID) override
    {
        if (!activeConnection.getComponent() && ID != MouseHoverExitDelay)
            return;

        switch (ID) {
        case RepaintTimer: {
            updateTextString();
            break;
            }
        case MouseHoverDelay: {
            setVisible(activeConnection.getComponent());
            break;
        }
        case MouseHoverExitDelay: {
            mouseDelay = 500;
            stopTimer(MouseHoverExitDelay);
            break;
        }
        }
    }

    void paint(Graphics& g) override
    {
        Path backgroundShadow;
        auto internalBounds = getLocalBounds().reduced(8);
        backgroundShadow.addRoundedRectangle(internalBounds, Corners::defaultCornerRadius);
        StackShadow::renderDropShadow(g, backgroundShadow, Colour(0, 0, 0).withAlpha(0.4f), 8, { 0, 0 });
        g.setColour(findColour(PlugDataColour::dialogBackgroundColourId));
        g.fillRoundedRectangle(internalBounds.toFloat(), Corners::defaultCornerRadius);

        // indicator - TODO
        //if(activeConnection.getComponent()) {
        //    Path indicatorPath;
        //    indicatorPath.addPieSegment(circlePosition.x - circleRadius,
        //                          circlePosition.y - circleRadius,
        //                          circleRadius * 2.0f,
        //                          circleRadius * 2.0f, 0, (activeConnection->messageActivity * (1.0f / 12.0f)) * MathConstants<float>::twoPi, 0.5f);
        //    g.setColour(findColour(PlugDataColour::panelTextColourId));
        //    g.fillPath(indicatorPath);
        //}
        Fonts::drawStyledText(g, textString, 8 + 4, 0, getWidth(), getHeight(), findColour(PlugDataColour::panelTextColourId), FontStyle::Regular, 14, Justification::centredLeft);
    }

    PluginEditor* editor;
    static inline bool isShowing = false;
    String textString;
    Component::SafePointer<Connection> activeConnection;
    int mouseDelay = 500;
    enum TimerID {RepaintTimer, MouseHoverDelay, MouseHoverExitDelay};

    Point<float> circlePosition = { 8 + 4, 36 / 2 };
    float circleRadius = 3.0f;
};
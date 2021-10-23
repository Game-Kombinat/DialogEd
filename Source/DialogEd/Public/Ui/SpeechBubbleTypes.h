#pragma once

/**
 * We quarter the screen into 4 parts, based on the average center position
 * of all actors that are part of a particular story thread.
 * The enum helps in creating a bitmask which part of the screen we're at.
 */
UENUM()
enum class EBubblePosition : uint8 {
    None = 0,
    Top = 1 << 1,
    Bottom = 1 << 2,
    Left = 1 << 3,
    Right = 1 << 4,

    TopLeft = Top | Left,
    TopRight = Top | Right,
    BottomLeft = Bottom | Left,
    BottomRight = Bottom | Right,
};

/**
 * Names the states a speechbubble can be in.
 */
UENUM()
enum class EBubbleState : uint8 {
    None,
    Opening,
    Writing, // This is for later when we actually have a typewriter thing going on.
    Waiting,
    Closing,
    Closed
};
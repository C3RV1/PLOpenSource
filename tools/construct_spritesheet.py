from sprite.SpriteLoader import SpriteLoaderOS
from sprite.Sprite import Frame, Tag
import os
import struct


def main(path: str):
    sprite_loader_os = SpriteLoaderOS("input")
    frame_info, tag_info = sprite_loader_os.load(path)
    endian = "@"

    with open(os.path.join("output", os.path.splitext(os.path.basename(path))[0]) + ".dat", "wb") as f:
        f.write(b"SPR\x20")
        f.write(struct.pack(endian + "BB", len(frame_info), len(tag_info)))
        for frame in frame_info:
            frame: Frame
            f.write(struct.pack(endian + "HHHH", frame.x, frame.y, frame.w, frame.h))
        for tag in tag_info:
            tag: Tag
            f.write(struct.pack(endian + "hhH", tag.childOffX, tag.childOffY, tag.childOffY))
            f.write(tag.name.encode("ascii") + b"\x00")
            f.write(struct.pack(endian + "B", len(tag.frames)))
            for frame in tag.frames:
                f.write(struct.pack(endian + "B", frame))
            for frameDuration in tag.frame_durations:
                f.write(struct.pack(endian + "f", frameDuration))


if __name__ == "__main__":
    main("headphones_play.png.json")

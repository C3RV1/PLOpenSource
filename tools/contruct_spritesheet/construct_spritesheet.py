from sprite.SpriteLoader import SpriteLoaderOS
from sprite.Sprite import Frame, Tag
import os
import struct
import argparse


def main():
    parser = argparse.ArgumentParser(description="Convert a Aseprite JSON Spritesheet to k4sdl format")
    parser.add_argument("input_path", help="Input file (JSON from Aseprite w/ array option)")
    parser.add_argument("output_path", help="Output file (.dat preferably)")
    args = parser.parse_args()

    sprite_loader_os = SpriteLoaderOS()
    frame_info, tag_info = sprite_loader_os.load(args.input_path)
    if frame_info is None:
        print(f"File {args.input_path} not found")
        return
    endian = "@"

    with open(args.output_path, "wb") as f:
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
    main()

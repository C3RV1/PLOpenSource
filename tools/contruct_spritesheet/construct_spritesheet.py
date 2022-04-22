from pg_engine.sprite.SpriteLoader import SpriteLoaderOS
from pg_engine.sprite.Sprite import Frame, Tag
import os
import struct
import argparse
import pickle
import base64


def main():
    parser = argparse.ArgumentParser(description="Convert a Aseprite JSON Spritesheet to k4sdl format")
    parser.add_argument("input_path", help="Input file (JSON from Aseprite w/ array option)")
    parser.add_argument("output_path", help="Output file (.dat preferably)")
    parser.add_argument("--pickle", help="Interpret input_path as pickle data of a Tuple[List[Frame], List[Tag]]",
                        action="store_true")
    args = parser.parse_args()

    if args.pickle:
        frame_info, tag_info = pickle.loads(base64.b64decode(args.input_path.encode("ascii")))
    else:
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
            f.write(struct.pack(endian + "hhH", tag.child_x, tag.child_y, tag.child_index))
            f.write(tag.name.encode("ascii") + b"\x00")
            f.write(struct.pack(endian + "B", len(tag.frames)))
            for frame in tag.frames:
                f.write(struct.pack(endian + "B", frame))
            for frameDuration in tag.frame_durations:
                f.write(struct.pack(endian + "f", frameDuration))


if __name__ == "__main__":
    main()

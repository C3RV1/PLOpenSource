import os
from .Sprite import Tag, Frame
import json


class SpriteLoader:
    def load(self, path: str):
        pass


class SpriteLoaderOS(SpriteLoader):
    def __init__(self, base_path_os=None):
        self.base_path = base_path_os

    def load(self, path: str):
        if self.base_path:
            path = os.path.join(self.base_path, path)

        if not os.path.isfile(path):
            return None, None
            
        frame_info = []
        tag_info = []
        with open(path, "r") as json_file:
            data = json.loads(json_file.read())

        for frame in data["frames"]:
            frame_data = frame["frame"]
            frame_info.append(Frame(frame_data["x"], frame_data["y"], frame_data["w"], frame_data["h"]))

        for tag in data["meta"]["frameTags"]:
            frames = []
            frame_durations = []
            if "frames" in tag:
                frames = tag["frames"]
            elif "from" in tag and "to" in tag:
                frames = list(range(tag["from"], tag["to"] + 1))
            if "durations" in tag:
                for duration in tag["durations"]:
                    frame_durations.append(duration / 1000.0)
            else:
                for frame in frames:
                    frame_durations.append(data["frames"][frame]["duration"] / 1000.0)
            tag_info.append(Tag(tag["name"], frames, frame_durations, 0, 0, 0))
        
        return frame_info, tag_info

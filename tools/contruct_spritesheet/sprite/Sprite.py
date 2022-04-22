from dataclasses import dataclass
from typing import List


@dataclass
class Frame:
    x: int
    y: int
    w: int
    h: int

    @property
    def position(self):
        return self.x, self.y

    @property
    def size(self):
        return self.w, self.h


@dataclass
class Tag:
    name: str
    frames: List[int]
    frame_durations: List[float]
    childOffX: int
    childOffY: int
    childTag: int

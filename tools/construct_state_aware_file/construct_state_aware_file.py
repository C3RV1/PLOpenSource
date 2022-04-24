import struct


class SAFConstructor:
    def __init__(self):
        self.conditions = b""
        self.segments = []

    def clear(self):
        self.conditions = b""
        self.segments = []

    def construct(self):
        data = b"SAFF"
        data += struct.pack("@B", len(self.segments))
        for segment in self.segments:
            data += struct.pack("@H", len(segment))
            data += segment
        data += self.conditions
        return data
    
    def add_segment(self, segment):
        self.segments.append(segment)

    def construct_cond(self, op, comparator, field, value):
        constructed = 0b10000000
        constructed |= op << 5
        # set 11101000
        if op < 2:
            comparator_table = {
                "==": [0, 0],
                "!=": [0, 1],
                ">": [1, 0],
                "<=": [1, 1],
                ">=": [2, 0],
                "<": [2, 1]
            }
            comp_value, comp_neg = comparator_table[comparator]
            constructed |= comp_value << 1
            constructed |= comp_neg << 4
            field_table = {
                "chapter": 0,
                "stage": 1
            }
            field_value = field_table[field]
            # set 11111110
            if value > 255:
                constructed |= 1
                self.conditions += struct.pack("@BBH", constructed, field_value, value)
            else:
                self.conditions += struct.pack("@BBB", constructed, field_value, value)
            # set 11111111
        else:
            self.conditions += struct.pack("@B", constructed)
    
    def construct_flag(self, op, check_set, type_, num):
        constructed = 0b10001000
        constructed |= op << 5
        # set 0b11101000
        if not check_set:
            constructed |= 0b10000
        # set 0b11111000
        type_table = {
            "coin": 0,
            "puzzle_found": 1,
            "puzzle_solved": 2,
            "flag": 3
        }
        constructed |= type_table[type_]
        # set 0b11111111
        self.conditions += struct.pack("@BH", constructed, num)

    def ifcond(self, comparator, field, value):
        self.construct_cond(0, comparator, field, value)
        return True

    def ifflag(self, check_set, type_, num):
        self.construct_flag(0, check_set, type_, num)
        return True
    
    def elifcond(self, comparator, field, value):
        self.construct_cond(1, comparator, field, value)
        return True

    def elifflag(self, check_set, type_, num):
        self.construct_flag(1, check_set, type_, num)
        return True

    def else_(self):
        self.construct_cond(2, 0, 0, 0)
        return True
    
    def endif(self):
        self.construct_cond(3, 0, 0, 0)
        return True
    
    def write_segment(self, num):
        num &= 0b1111111
        self.conditions += struct.pack("@B", num)


if __name__ == "__main__":
    c = SAFConstructor()

    c.write_segment(0)
    if c.ifcond("==", "chapter", 0):
        c.write_segment(1)
    if c.elifcond(">", "chapter", 1):
        if c.ifcond("<=", "stage", 1):
            c.write_segment(2)
        if c.else_():
            c.write_segment(3)
        c.endif()
    if c.else_():
        c.write_segment(4)
    c.endif()
    c.write_segment(5)

    c.add_segment(b"Hello ")
    c.add_segment(b"World!")
    c.add_segment(b"Game!")
    c.add_segment(b"Layton!")
    c.add_segment(b"added?")
    c.add_segment(b" end\0")
    print(c.conditions)
    cons = c.construct()
    with open("output/constructed.dat", "wb") as f:
        f.write(cons)

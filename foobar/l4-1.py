class Graph (object):
    def __init__(self, sources, sinks, capacity):
        limit = 2000000
        self.size = len(capacity)
        self.capacity = capacity
        multiSource = len(sources) > 1
        multiSink = len(sinks) > 1
        offset = 0
        if (multiSource):
            offset = 1
            self.size += 1
            for row in self.capacity:
                row.insert(0, 0)
            self.capacity.insert(0, [0] * self.size)
            for source in sources:
                self.capacity[0][source + offset] = limit
        if (multiSink):
            self.size += 1
            for row in self.capacity:
                row.append(0)
            self.capacity.append([0] * self.size)
            for sink in sinks:
                self.capacity[sink + offset][self.size - 1] = limit
        self.flow = []
        for i in range(0, self.size):
            self.flow.append([0] * self.size)
        self.excess = [0] * self.size
        self.distance = [0] * self.size
        self.seen = [0] * self.size
        self.source = 0
        self.sink = self.size - 1

    def _push(self, u, v):
        delta_flow = min(self.excess[u], (self.capacity[u][v] - self.flow[u][v]))
        self.flow[u][v] += delta_flow
        self.flow[v][u] -= delta_flow
        self.excess[u] -= delta_flow
        self.excess[v] += delta_flow

    def _relabel(self, u):
        min_distance = float('inf')
        for v in range(self.size):
            if (self.capacity[u][v] - self.flow[u][v]) > 0 :
                min_distance = min(min_distance, self.distance[v])
                self.distance[u] = min_distance + 1

    def _discharge(self, u):
        while self.excess[u] > 0 :
            if self.seen[u] < self.size :
                v = self.seen[u]
                if (self.capacity[u][v] - self.flow[u][v] > 0) and (self.distance[u] > self.distance[v]):           
                    self._push(u, v)
                else:
                    self.seen[u] += 1             
            else:
                self._relabel(u)
                self.seen[u] = 0

    def pushRelable(self):
        nodes = range(1, self.size - 1)
        self.distance[self.source] = self.size
        self.excess[self.source] = float('inf')
        for v in range(self.size):
            self._push(self.source, v)
        potential_vertex = 0

        while potential_vertex < len(nodes):
            u = nodes[potential_vertex]
            pred_distance = self.distance[u]
            self._discharge(u)
            if self.distance[u] > pred_distance :
                nodes.insert(0, nodes.pop(potential_vertex))
                potential_vertex = 0
            else:
                potential_vertex += 1

        return sum (self.flow[self.source])

def solution(sources, sinks, capacity):
    return Graph(sources, sinks, capacity).pushRelable()

print solution([0], [5], [
    [0, 10, 0, 8, 0, 0],
    [0, 0, 5, 2, 0, 0],
    [0, 0, 0, 0, 0, 7],
    [0, 0, 0, 0, 10, 0],
    [0, 0, 8, 0, 0, 10],
    [0, 0, 0, 0, 0, 0]
])

print solution([0], [3], [
    [0, 7, 0, 0],
    [0, 0, 6, 0],
    [0, 0, 0, 8],
    [9, 0, 0, 0]
])

print solution([0, 1], [4, 5], [
    [0, 0, 4, 6, 0, 0],
    [0, 0, 5, 2, 0, 0],
    [0, 0, 0, 0, 4, 4],
    [0, 0, 0, 0, 6, 6],
    [0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0]
])

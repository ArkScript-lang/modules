import time
import urllib.request as req
from multiprocessing import Process, Queue
import statistics as stat


def job(q):
    start = time.time()
    f = req.urlopen('http://localhost:80/hi')
    f.read().decode('utf-8')
    q.put(time.time() - start)


if __name__ == '__main__':
    start = time.time()

    threads = []
    q = Queue()
    for i in range(5000):
        threads.append(Process(target=job, args=(q,)))
    print("creation finished at", time.time() - start, "s")
    for t in threads:
        t.start()
        t.join()

    print("total time", time.time() - start, "s")

    print("\n\n")

    e = []
    while not q.empty():
        e.append(q.get() * 1000)
    print("mean       ", stat.mean(e), "ms")
    print("meadian    ", stat.median(e), "ms")
    print("min        ", min(e), "ms")
    print("max        ", max(e), "ms")
    print("std dev    ", stat.stdev(e))
    print("variance   ", stat.variance(e))
    print("median low ", stat.median_low(e), "ms")
    print("median high", stat.median_high(e), "ms")
    print("mode       ", stat.mode(e), "s")
    #print(e)
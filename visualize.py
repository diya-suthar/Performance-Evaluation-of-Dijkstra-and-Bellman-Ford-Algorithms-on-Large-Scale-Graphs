import pandas as pd
import matplotlib.pyplot as plt
import networkx as nx

df = pd.read_csv("road_network_large.csv")

G = nx.Graph()
for _, row in df.iterrows():
    G.add_edge(row["source"], row["target"], weight=row["weight"])

pos = nx.kamada_kawai_layout(G)

plt.figure(figsize=(10,8))
nx.draw(G, pos, node_size=50, width=1.5, with_labels=False)

plt.title("Vadodara Road Network")
plt.axis("off")

# IMPORTANT: save image for HTML
plt.savefig("vadodara_map.png", dpi=300)

plt.show()
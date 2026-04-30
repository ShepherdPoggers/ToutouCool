import numpy as np
import matplotlib.pyplot as plt

# ================== PARAMÈTRES ==================
minDelay = 4
maxDelay = 18
servoDelay = maxDelay

stress = 0.7
recovery_rate = 0.03

total_time = 20
dt = 0.02

# ================== SÉQUENCES ==================
# Séquence 1 (longue)
seq1 = np.arange(5, 7, 0.12)   # ~17 impulsions

# Séquence 2 (plus courte + chevauchement)
seq2 = np.arange(6, 6.8, 0.08) # ~10 impulsions

# Fusion (chevauchement inclus)
tilt_events = np.concatenate([seq1, seq2])
tilt_events.sort()

# ================== SIMULATION ==================
times = np.arange(0, total_time, dt)
respiration_times = []

tilt_triggered_times = []
tilt_triggered_values = []

for t in times:
    # 🔴 Détection précise (possibilité de double tilt au même moment)
    hits = [ev for ev in tilt_events if abs(t - ev) < dt/2]

    if hits:
        for _ in hits:  # plusieurs tilts possibles en même temps
            if (servoDelay - stress) > minDelay:
                servoDelay -= stress

            respiration = (360 * servoDelay) / 1000

            tilt_triggered_times.append(t)
            tilt_triggered_values.append(respiration)

    # 🔵 Rétablissement
    if servoDelay < maxDelay:
        servoDelay += recovery_rate

    respiration = (360 * servoDelay) / 1000
    respiration_times.append(respiration)

# ================== GRAPHIQUE ==================
plt.figure(figsize=(11, 5))

plt.plot(times, respiration_times, label="Secondes / respiration")

plt.scatter(tilt_triggered_times, tilt_triggered_values,
            color='red', s=20, label="Activation tilt", zorder=3)

plt.xlabel("Temps (s)")
plt.ylabel("Secondes / respiration")
plt.title("Simulation avec séquences de tilt chevauchées")
plt.legend()
plt.grid()

plt.show()

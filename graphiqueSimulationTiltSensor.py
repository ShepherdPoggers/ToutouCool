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
seq1 = np.arange(5, 7, 0.12)   # longue
seq2 = np.arange(6, 6.8, 0.08) # courte (chevauchement)

tilt_events = np.concatenate([seq1, seq2])
tilt_events.sort()

# ================== SIMULATION ==================
times = np.arange(0, total_time, dt)
respiration_times = []

tilt_triggered_times = []
tilt_triggered_values = []

for t in times:
    hits = [ev for ev in tilt_events if abs(t - ev) < dt/2]

    if hits:
        for _ in hits:
            if (servoDelay - stress) > minDelay:
                servoDelay -= stress

            respiration = (360 * servoDelay) / 1000
            tilt_triggered_times.append(t)
            tilt_triggered_values.append(respiration)

    if servoDelay < maxDelay:
        servoDelay += recovery_rate

    respiration = (360 * servoDelay) / 1000
    respiration_times.append(respiration)

# ================== STYLE (TEXTE x2) ==================
plt.figure(figsize=(12, 6))

plt.plot(times, respiration_times, linewidth=2.5,
         label="Secondes / respiration")

plt.scatter(tilt_triggered_times, tilt_triggered_values,
            s=60,  # points plus gros
            label="Activation tilt", zorder=3, color='red')

plt.xlabel("Temps (s)", fontsize=18)
plt.ylabel("Secondes / respiration", fontsize=18)
plt.title("Simulation avec séquences de tilt chevauchées", fontsize=20)

plt.xticks(fontsize=14)
plt.yticks(fontsize=14)

plt.legend(fontsize=14)
plt.grid()

plt.tight_layout()
plt.show()

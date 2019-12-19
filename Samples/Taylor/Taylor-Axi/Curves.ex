# Global parameters
Parameters, xname=$Time\ (s)$, marksnumber=15, title=$Taylor\ Axi\ Benchmark\ Test$, crop=True

# Temperature curve
Temperature, yname=$Temperature\ (^{\circ}C)$, legendlocate=bottomright, name=$DynELA\ T$, temperature.plot, name=$Abaqus\ T$, Abaqus/Taylor_Temperature.plot

# Plastic strain curve
Peeq, yname=$Equivalent\ plastic\ strain\ \overline{\varepsilon}^{p}$, legendlocate=bottomright, name=$DynELA\ \overline{\varepsilon}^{p}$, plasticStrain.plot, name=$Abaqus\ \overline{\varepsilon}^{p}$, Abaqus/Taylor_PlasticStrain.plot

# von Mises equivalent stress curve
Mises, yname=$von\ Mises\ stress\ \overline{\sigma}\ (MPa)$, legendlocate=topright, name=$DynELA\ \overline{\sigma}$, vonMises.plot, name=$Abaqus\ \overline{\sigma}$, Abaqus/Taylor_vonMises.plot

# TimeStep curve
TimeStep, yname=$Time\ increment\ \Delta t\ (s)$, legendlocate=topright, name=$DynELA\ \Delta t$, dt.plot, name=$Abaqus\ \Delta t$, Abaqus/Taylor_timeStep.plot

# Kinetic energy curve
KineticEnergy, yname=$Kinetic\ energy$, legendlocate=topright, name=$DynELA\ Kinetic\ Energy$, ke.plot, name=$Abaqus\ Kinetic\ Energy$, Abaqus/Taylor_kineticEnergy.plot

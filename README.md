# Ejercicio 2: Gastos Personales - Arquitectura C++20

El diseño propuesto utiliza tres patrones para lograr un desacoplamiento total en diferentes niveles del sistema:

1. **Strategy (std::ranges::sort + Lambdas):** Desacopla la lógica de *ordenamiento* del flujo principal. Permite inyectar dinámicamente diferentes criterios (por monto o por categoría/fecha) sin tener que modificar la estructura de los datos ni escribir múltiples sentencias `if/else`.
2. **Factory Method (make_exporter + CTAD):** Desacopla la *creación* de los exportadores (CSV, JSON, Text). El cliente no necesita conocer la implementación exacta de cada formato de salida; solo invoca a la fábrica genérica que retorna un `std::function` estandarizado.
3. **Decorator (Composición por valor y Concepts):** Desacopla las *responsabilidades adicionales* (como el cálculo del resumen total y la auditoría) del formato de exportación base. Permite apilar o "envolver" comportamientos sobre cualquier clase que cumpla con el concepto `ExpenseExporter`, evitando la herencia múltiple y manteniendo las clases originales intactas (Cumpliendo el principio Open/Closed).
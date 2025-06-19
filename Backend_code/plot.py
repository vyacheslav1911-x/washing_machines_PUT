import pandas as pd
import plotly.graph_objs as go
from plotly.subplots import make_subplots

# Завантаж CSV
df = pd.read_csv('mpu6050.csv')
df['timestamp'] = pd.to_datetime(df['timestamp'])

# Побудова інтерактивного графіка
fig = make_subplots(rows=1, cols=1)

fig.add_trace(go.Scatter(x=df['timestamp'], y=df['X'], mode='lines', name='X'))
fig.add_trace(go.Scatter(x=df['timestamp'], y=df['Y'], mode='lines', name='Y'))
fig.add_trace(go.Scatter(x=df['timestamp'], y=df['Z'], mode='lines', name='Z'))

fig.update_layout(
    title='Інтерактивний графік XYZ',
    xaxis_title='Час',
    yaxis_title='Значення',
    height=800,
    width=1400,
    xaxis_rangeslider_visible=True,
    legend_title_text='Вісі'
)

fig.write_html("your2_graph.html")
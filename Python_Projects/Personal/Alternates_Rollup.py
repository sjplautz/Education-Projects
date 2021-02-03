# Import the necessary packages for operation
import openpyxl
import pandas as pd
import time

# Open the specified CSV
data = pd.read_csv('AlternatesWorkbookRAW.csv')

# Get the markup to apply to costs
markup = data.iloc[0, 4]

# Aggregate the alternate cost numbers
rollup = data.groupby(['ALTERNATE NUMBER'])[['ITEM COST']].sum()

# Apply currency formatting to costs
def format(val):
    return "${:,.0f}".format(val * markup)

rollup['ITEM COST'] = rollup['ITEM COST'].apply(format)

# Apply coloring to costs
def coloring(val):
    color = 'red' if val[1] == "-" else 'green'
    return 'color: %s' % color

rollup = rollup.style.applymap(coloring, subset=['ITEM COST'])

# Create output filename based on current date/time
todaysDate = time.strftime("%h-%d-%Y")
excelFilename = "Alternates_Rollup:" + todaysDate+ ".xlsx"

# Write the data to an excel
rollup.to_excel(excelFilename, encoding='utf8')

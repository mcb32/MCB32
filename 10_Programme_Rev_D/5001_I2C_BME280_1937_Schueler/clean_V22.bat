
@echo OFF
@echo ************************************************
@echo Loescht unnoetige Files aus den Projektordnern
@echo ************************************************
@pause
@echo
del *.o /s
del *.i /s
del stm3*.txt /s
del dma*.txt /s
del gpio*.txt /s
del system*.txt /s
del dac*.txt /s
del adc*.txt

del *.bak /s
del *.htm /s
del *.map /s
del *.sct /s
del *.dep /s
del *.crf /s
del *.hex /s
del *.axf /s
del *.build_log.htm /s

del *.d /s
del *.lnp /s
del *.plg /s
del *.out /s
del *log*.txt /s
del *.lst /s
del *.uvgui.* /s
del *.uvguix.* /s
del *.uvguix.rma* /s
del *.build_log.htm  /s
del *.uvproj.saved*  /s

@Echo  ---------------------------
@Echo  Druecke CTRL_C fuer Abbruch
@Echo  ---------------------------
@Echo OFF
pause

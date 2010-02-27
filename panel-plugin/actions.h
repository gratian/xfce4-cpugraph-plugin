#include <cpu.h>

void AssociateCommandChange( GtkEntry *entry, CPUGraph *base );
void ChangeColor1( GtkColorButton * button, CPUGraph * base );
void ChangeColor2( GtkColorButton *button, CPUGraph *base );
void ChangeColor3( GtkColorButton *button, CPUGraph *base );
void ChangeColor4( GtkColorButton *button, CPUGraph *base );
void ColorModeChange( GtkOptionMenu *om, CPUGraph *base );
void DialogResponse( GtkWidget *dlg, int response, CPUGraph *base );
void ApplyChanges( CPUGraph *base );
void FrameChange( GtkToggleButton *button, CPUGraph *base );
void ModeChange( GtkOptionMenu *om, CPUGraph *base );
void WidthChange( GtkSpinButton *sb, CPUGraph *base );
void TimeScaleChange( GtkToggleButton *button, CPUGraph *base );
void UpdateChange( GtkOptionMenu *om, CPUGraph *base );

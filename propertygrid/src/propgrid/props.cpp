/////////////////////////////////////////////////////////////////////////////
// Name:        props.cpp
// Purpose:     Basic Property Classes
// Author:      Jaakko Salli
// Modified by:
// Created:     May-14-2004
// RCS-ID:      $Id:
// Copyright:   (c) Jaakko Salli
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/defs.h"
    #include "wx/object.h"
    #include "wx/hash.h"
    #include "wx/string.h"
    #include "wx/log.h"
    #include "wx/event.h"
    #include "wx/window.h"
    #include "wx/panel.h"
    #include "wx/dc.h"
    #include "wx/dcclient.h"
    #include "wx/dcmemory.h"
    #include "wx/button.h"
    #include "wx/pen.h"
    #include "wx/brush.h"
    #include "wx/cursor.h"
    #include "wx/dialog.h"
    #include "wx/settings.h"
    #include "wx/msgdlg.h"
    #include "wx/choice.h"
    #include "wx/stattext.h"
    #include "wx/scrolwin.h"
    #include "wx/dirdlg.h"
    #include "wx/combobox.h"
    #include "wx/layout.h"
    #include "wx/sizer.h"
    #include "wx/textdlg.h"
    #include "wx/filedlg.h"
    #include "wx/statusbr.h"
    #include "wx/intl.h"
#endif

#include <wx/filename.h>

#include <wx/propgrid/propgrid.h>

#include <wx/propgrid/propdev.h>


#define wxPG_CUSTOM_IMAGE_WIDTH     20 // for wxColourProperty etc.


// -----------------------------------------------------------------------
// wxStringProperty
// -----------------------------------------------------------------------

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxStringProperty,wxBaseProperty,
                               wxString,const wxString&,TextCtrl)

wxStringPropertyClass::wxStringPropertyClass ( const wxString& label, const wxString& name,
    const wxString& value ) : wxPGProperty(label,name)
{
    DoSetValue(value);
}

wxStringPropertyClass::~wxStringPropertyClass () { }

void wxStringPropertyClass::DoSetValue ( wxPGVariant value )
{
    m_value = wxPGVariantToString(value);
}

wxPGVariant wxStringPropertyClass::DoGetValue () const
{
    return wxPGVariant(m_value);
}

wxString wxStringPropertyClass::GetValueAsString ( int ) const
{
    return m_value;
}

bool wxStringPropertyClass::SetValueFromString ( const wxString& text, int )
{
    if ( m_value != text )
        return StdValidationProcedure(text);

    return false;
}

/*
bool wxStringPropertyClass::OnEvent ( wxPropertyGrid* propgrid, wxWindow* primary,
    wxEvent& event )
{
    if ( event.GetEventType() == wxEVT_COMMAND_BUTTON_CLICKED )
    {
        // Update the value
        PrepareValueForDialogEditing(propgrid);

        wxString val1 = GetValueAsString(0);
        wxString val_orig = val1;

        wxString value;
        wxPropertyGrid::ExpandEscapeSequences(value,val1);

        // Run editor dialog.
        if ( OnButtonClick(propgrid,value) )
        {
            wxPropertyGrid::CreateEscapeSequences(val1,value);
            if ( val1 != val_orig )
            {
                SetValueFromString ( val1, 0 );
                UpdateControl ( primary );
                return true;
            }
        }
    }
    return false;
}

bool wxStringPropertyClass::OnButtonClick ( wxPropertyGrid* propgrid, wxString& value )
{
    // launch editor dialog
    wxDialog* dlg = new wxDialog (propgrid,-1,m_label,
        wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxCLIP_CHILDREN);

    // Multi-line text editor dialog.
    const int spacing = 8;
    wxBoxSizer* topsizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer* rowsizer = new wxBoxSizer( wxHORIZONTAL );
    wxTextCtrl* ed = new wxTextCtrl(dlg,11,value,
        wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE);
    rowsizer->Add ( ed, 1, wxEXPAND|wxALL, spacing );
    topsizer->Add ( rowsizer, 1, wxEXPAND, 0 );
    rowsizer = new wxBoxSizer( wxHORIZONTAL );
    const int but_sz_flags =
        wxALIGN_RIGHT|wxALIGN_CENTRE_VERTICAL|wxBOTTOM|wxLEFT|wxRIGHT;
    rowsizer->Add ( new wxButton(dlg,wxID_OK,_("Ok")),
        0, but_sz_flags, spacing );
    rowsizer->Add ( new wxButton(dlg,wxID_CANCEL,_("Cancel")),
        0, but_sz_flags, spacing );
    topsizer->Add ( rowsizer, 0, wxALIGN_RIGHT|wxALIGN_CENTRE_VERTICAL, 0 );

    dlg->SetSizer ( topsizer );
    topsizer->SetSizeHints( dlg );

    dlg->SetSize (400,300);

    dlg->Move ( propgrid->GetGoodEditorDialogPosition (this,dlg->GetSize()) );

    int res = dlg->ShowModal();

    if ( res == wxID_OK )
    {
        value = ed->GetValue();
        dlg->Destroy();
        return true;
    }
    dlg->Destroy();
    return false;
}
*/

void wxStringPropertyClass::SetAttribute ( int id, wxVariant& value )
{
    if ( id == wxPG_STRING_PASSWORD )
    {
        m_flags &= ~(wxPG_PROP_PASSWORD);
        if ( value.GetLong() ) m_flags |= wxPG_PROP_PASSWORD;
        RecreateEditor();
    }
}

// -----------------------------------------------------------------------
// wxIntProperty
// -----------------------------------------------------------------------

wxPG_BEGIN_PROPERTY_CLASS_BODY(wxIntProperty,wxPGProperty,long,long)
    WX_PG_DECLARE_BASIC_TYPE_METHODS()
    virtual bool SetValueFromInt ( long value, int flags );
wxPG_END_PROPERTY_CLASS_BODY()

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxIntProperty,wxBaseProperty,
                               long,long,TextCtrl)

wxIntPropertyClass::wxIntPropertyClass ( const wxString& label, const wxString& name,
    long value ) : wxPGProperty(label,name)
{
    DoSetValue(value);
}

wxIntPropertyClass::~wxIntPropertyClass () { }

void wxIntPropertyClass::DoSetValue ( wxPGVariant value )
{
    m_value = wxPGVariantToLong(value);
}

wxPGVariant wxIntPropertyClass::DoGetValue () const
{
    return wxPGVariant(m_value);
}

wxString wxIntPropertyClass::GetValueAsString ( int ) const
{
    return wxString::Format(wxT("%li"),m_value);
}

bool wxIntPropertyClass::SetValueFromString ( const wxString& text, int arg_flags )
{
    wxString s;
    long value;

    if ( text.IsNumber() )
    {
        text.ToLong(&value,0); // we know its number, so need to check retval

        if ( m_value != value )
        {
            return StdValidationProcedure(value);
        }
    }
    else if ( arg_flags & wxPG_REPORT_ERROR )
    {
        s.Printf ( wxT("! %s: \"%s\" is not a number."), m_label.c_str(), text.c_str() );
        ShowError(s);
    }
    return false;
}

bool wxIntPropertyClass::SetValueFromInt( long value, int WXUNUSED(flags) )
{
    if ( m_value != value )
    {
        m_value = value;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------
// wxUIntProperty
// -----------------------------------------------------------------------


#define wxPG_UINT_TEMPLATE_MAX 8

static const wxChar* gs_uintTemplates[wxPG_UINT_TEMPLATE_MAX] = {
    wxT("%x"),wxT("0x%x"),wxT("$%x"),
    wxT("%X"),wxT("0x%X"),wxT("$%X"),
    wxT("%u"),wxT("%o")
};

wxPG_BEGIN_PROPERTY_CLASS_BODY(wxUIntProperty,wxBasePropertyClass,long,unsigned long)
    WX_PG_DECLARE_BASIC_TYPE_METHODS()
    WX_PG_DECLARE_ATTRIBUTE_METHODS()
    virtual bool SetValueFromInt ( long value, int flags );
protected:
    wxByte      m_base;
    wxByte      m_realBase; // translated to 8,16,etc.
    wxByte      m_prefix;
wxPG_END_PROPERTY_CLASS_BODY()

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxUIntProperty,wxBaseProperty,
                               long,unsigned long,TextCtrl)

wxUIntPropertyClass::wxUIntPropertyClass ( const wxString& label, const wxString& name,
    unsigned long value ) : wxBasePropertyClass(label,name)
{
    m_base = 6; // This is magic number for dec base (must be same as in setattribute)
    m_realBase = 10;
    m_prefix = wxPG_PREFIX_NONE;

    DoSetValue((long)value);
}

wxUIntPropertyClass::~wxUIntPropertyClass () { }

void wxUIntPropertyClass::DoSetValue ( wxPGVariant value )
{
    m_value = wxPGVariantToLong(value);
}

wxPGVariant wxUIntPropertyClass::DoGetValue () const
{
    return wxPGVariant(m_value);
}

wxString wxUIntPropertyClass::GetValueAsString ( int ) const
{
    //return wxString::Format(wxPGGlobalVars->m_uintTemplate.c_str(),m_value);

    size_t index = m_base + m_prefix;
    if ( index >= wxPG_UINT_TEMPLATE_MAX )
        index = wxPG_BASE_DEC;

    return wxString::Format(gs_uintTemplates[index],m_value);
}

bool wxUIntPropertyClass::SetValueFromString ( const wxString& text, int WXUNUSED(arg_flags) )
{
    wxString s;
    long value = 0;

    const wxChar *start = text.c_str();
    if ( text[0] && !wxIsalnum(text[0]) )
        start++;

    wxChar *end;
    value = wxStrtoul(start, &end, (unsigned int)m_realBase);

    //wxLogDebug(wxT("\"%s\" (%u) realbase=%u, flags=0x%X"),start,(unsigned int)value,(unsigned int)m_realBase,m_flags);

    if ( m_value != value )
    {
        return StdValidationProcedure(value);
    }
    /*}
    else if ( arg_flags & wxPG_REPORT_ERROR )
    {
        s.Printf ( wxT("! %s: \"%s\" is not a number."), m_label.c_str(), text.c_str() );
        ShowError(s);
    }*/
    return false;
}

bool wxUIntPropertyClass::SetValueFromInt( long value, int WXUNUSED(flags) )
{
    if ( m_value != value )
    {
        m_value = value;
        return true;
    }
    return false;
}

void wxUIntPropertyClass::SetAttribute ( int id, wxVariant& value )
{
    if ( id == wxPG_UINT_BASE )
    {
        int val = value.GetLong();

        m_realBase = (wxByte) val;
        if ( m_realBase > 16 )
            m_realBase = 16;

        //
        // Translate logical base to a template array index
        m_base = 7; // oct
        if ( val == wxPG_BASE_HEX )
            m_base = 3;
        else if ( val == wxPG_BASE_DEC )
            m_base = 6;
        else if ( val == wxPG_BASE_HEXL )
            m_base = 0;
    }
    else if ( id == wxPG_UINT_PREFIX )
        m_prefix = (wxByte) value.GetLong();
}

/*
void wxUIntPropertyClass::SetAttribute ( int id, wxVariant& value )
{
    wxString prefix;

    if ( id == wxPG_UINT_RADIX )
    {
        wxPGGlobalVars->m_uintRadix = value.GetLong();

        prefix = wxPGGlobalVars->m_uintTemplate.substr(0,
            wxPGGlobalVars->m_uintTemplate.length()-2);
    }
    else if ( id == wxPG_UINT_PREFIX )
    {
        prefix = value.GetString();
    }

    //
    // Reconstruct sprintf template

    int radix = wxPGGlobalVars->m_uintRadix;

    const wxChar* radixStr = wxT("%u");
    if (radix==16) radixStr = wxT("%X");
    else if (radix==8) radixStr = wxT("%o");
    else if (radix==32) radixStr = wxT("%x");

    prefix.append(radixStr);
    wxPGGlobalVars->m_uintTemplate = prefix;

}
*/

// -----------------------------------------------------------------------
// wxFloatProperty
// -----------------------------------------------------------------------

wxPG_BEGIN_PROPERTY_CLASS_BODY(wxFloatProperty,wxPGProperty,double,double)
    WX_PG_DECLARE_BASIC_TYPE_METHODS()
    WX_PG_DECLARE_ATTRIBUTE_METHODS()
protected:
    int m_precision;
wxPG_END_PROPERTY_CLASS_BODY()

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxFloatProperty,wxBaseProperty,
                               double,double,TextCtrl)

wxFloatPropertyClass::wxFloatPropertyClass ( const wxString& label, const wxString& name,
    double value ) : wxPGProperty(label,name)
{
    m_precision = -1;
    DoSetValue(value);
}

wxFloatPropertyClass::~wxFloatPropertyClass () { }

void wxFloatPropertyClass::DoSetValue ( wxPGVariant value )
{
    m_value = wxPGVariantToDouble(value);
}

wxPGVariant wxFloatPropertyClass::DoGetValue () const
{
    return wxPGVariant(m_value);
}

// This helper method provides standard way for floating point-using
// properties to convert values to string.
void wxPropertyGrid::DoubleToString(wxString& target,
                                    double value,
                                    int precision,
                                    bool removeZeroes,
                                    wxString* precTemplate)
{
    if ( precision >= 0 )
    {
        wxString text1;
        if (!precTemplate)
            precTemplate = &text1;

        if ( !precTemplate->length() )
        {
            *precTemplate = wxT("%.");
            *precTemplate << wxString::Format( wxT("%i"), precision );
            *precTemplate << wxT('f');
        }

        target.Printf ( precTemplate->c_str(), value );

    }
    else
    {
        target.Printf ( wxT("%f"), value );
    }

    if ( removeZeroes && precision != 0 )
    {
        wxASSERT ( target.Find(wxT('.')) != wxNOT_FOUND );

        // Remove excess zeroes (do not remove this code just yet)
        int cur_pos = target.length() - 1;
        wxChar a;
        a = target.GetChar ( cur_pos );
        while ( a == '0' && cur_pos > 0 )
        {
            cur_pos--;
            a = target.GetChar ( cur_pos );
        }

        if ( target.GetChar ( cur_pos ) != wxT('.') )
            cur_pos += 1;

        target.Truncate ( cur_pos );
    }
}

wxString wxFloatPropertyClass::GetValueAsString ( int arg_flags ) const
{
    wxString text;
    wxPropertyGrid::DoubleToString(text,m_value,
                                   m_precision,
                                   !(arg_flags & wxPG_FULL_VALUE),
                                   (wxString*) NULL);
    return text;
}

bool wxFloatPropertyClass::SetValueFromString ( const wxString& text, int arg_flags )
{
    wxString s;
    double value;
    bool res = text.ToDouble(&value);
    if ( res )
    {
#if wxPG_USE_VALIDATORS
        if ( m_validator )
        {
            m_validator->AssertDataType(wxT("double"));
            wxPGVariant tvariant(value);
            if ( !m_validator->Validate(tvariant,s) )
            {
                ShowError(s);
                return false;
            }
        }
#endif

        if ( m_value != value )
        {
            m_value = value;
            return true;
        }
    }
    else if ( arg_flags & wxPG_REPORT_ERROR )
    {
        s.Printf ( _("\"%s\" is not a floating-point number."), text.c_str() );
        ShowError(s);
    }
    return false;
}

void wxFloatPropertyClass::SetAttribute ( int id, wxVariant& value )
{
    if ( id == wxPG_FLOAT_PRECISION )
    {
        m_precision = value.GetLong();
    }
}

// -----------------------------------------------------------------------
// wxBoolProperty
// -----------------------------------------------------------------------

#define wxPG_PROP_USE_CHECKBOX      wxPG_PROP_CLASS_SPECIFIC_1
// DCC = Double Click Cycles
//#define wxPG_PROP_USE_DCC           wxPG_PROP_CLASS_SPECIFIC_2


wxPG_BEGIN_PROPERTY_CLASS_BODY2(wxBoolPropertyClass,wxPGProperty,bool,int,bool,class)
    WX_PG_DECLARE_BASIC_TYPE_METHODS()
    WX_PG_DECLARE_CHOICE_METHODS()
    WX_PG_DECLARE_ATTRIBUTE_METHODS()
wxPG_END_PROPERTY_CLASS_BODY()


// We cannot use standard WX_PG_IMPLEMENT_PROPERTY_CLASS macro, since
// there is a custom GetEditorClass.

WX_PG_IMPLEMENT_CONSTFUNC(wxBoolProperty,bool)
WX_PG_IMPLEMENT_CLASSINFO(wxBoolProperty,wxBasePropertyClass)
wxPG_GETCLASSNAME_IMPLEMENTATION(wxBoolProperty)
const wxPGValueType* wxBoolPropertyClass::GetValueType() const { return wxPG_VALUETYPE(bool); }

const wxPGEditor* wxBoolPropertyClass::DoGetEditorClass() const
{
    // Select correct editor control.
#if wxPG_INCLUDE_CHECKBOX
    if ( !(m_flags & wxPG_PROP_USE_CHECKBOX) )
        return wxPG_EDITOR(Choice);
    return wxPG_EDITOR(CheckBox);
#else
    return wxPG_EDITOR(Choice);
#endif
}

wxBoolPropertyClass::wxBoolPropertyClass ( const wxString& label, const wxString& name, bool value ) :
    wxPGProperty(label,name)
{
    int useval = 0;
    if ( value ) useval = 1;
    DoSetValue(useval);
}

wxBoolPropertyClass::~wxBoolPropertyClass () { }

void wxBoolPropertyClass::DoSetValue ( wxPGVariant value )
{
    m_value = 0;
    if ( wxPGVariantToLong(value) != 0 )
        m_value = 1;
}

wxPGVariant wxBoolPropertyClass::DoGetValue () const
{
    return wxPGVariant(m_value);
}

wxString wxBoolPropertyClass::GetValueAsString ( int arg_flags ) const
{
    if ( !(arg_flags & wxPG_FULL_VALUE) )
    {
        return wxPGGlobalVars->m_boolChoices[m_value];
    }
    wxString text;

    if (m_value) text = wxT("true");
    else text = wxT("false");

    return text;
}

int wxBoolPropertyClass::GetChoiceInfo ( wxPGChoiceInfo* choiceinfo )
{
    if ( choiceinfo )
    {
        choiceinfo->m_itemCount = 2;
        choiceinfo->m_arrWxString = wxPGGlobalVars->m_boolChoices;
    }
    return m_value;
}

bool wxBoolPropertyClass::SetValueFromString ( const wxString& text, int /*arg_flags*/ )
{
    int value = 0;
    if ( text.CmpNoCase(wxPGGlobalVars->m_boolChoices[1]) == 0 || text.CmpNoCase(wxT("true")) == 0 )
        value = 1;

    if ( m_value != value )
    {
        DoSetValue ( value );
        return true;
    }
    /*
    else if ( arg_flags & wxPG_REPORT_ERROR )
    {
        wxLogError ( wxT("Property %s: \"%s\" is not a boolean value (True and False are valid)."), m_label.c_str(), text.c_str() );
    }
    */
    return false;
}

bool wxBoolPropertyClass::SetValueFromInt ( long value, int )
{
    if ( value != 0 ) value = 1;

    if ( m_value != value )
    {
        m_value = value;
        return true;
    }
    return false;
}

void wxBoolPropertyClass::SetAttribute ( int id, wxVariant& value )
{
    int ival = value.GetLong();
#if wxPG_INCLUDE_CHECKBOX
    if ( id == wxPG_BOOL_USE_CHECKBOX )
    {
        if ( ival )
            m_flags |= wxPG_PROP_USE_CHECKBOX;
        else
            m_flags &= ~(wxPG_PROP_USE_CHECKBOX);
    }
    //else
#endif
    /*
    if ( id == wxPG_BOOL_USE_DOUBLE_CLICK_CYCLING )
    {
        if ( ival )
            m_flags |= wxPG_PROP_USE_DCC;
        else
            m_flags &= ~(wxPG_PROP_USE_DCC);
    }
    */
}

// -----------------------------------------------------------------------
// wxBaseEnumPropertyClass
// -----------------------------------------------------------------------

// Class body is in propdev.h.

wxBaseEnumPropertyClass::wxBaseEnumPropertyClass( const wxString& label, const wxString& name )
    : wxPGProperty(label,name)
{
}

/** If has values array, then returns number at index with value -
    otherwise just returns the value.
*/
int wxBaseEnumPropertyClass::GetIndexForValue( int value ) const
{
    return value;
}

void wxBaseEnumPropertyClass::DoSetValue ( wxPGVariant value )
{
    //wxLogDebug(wxT("wxBaseEnumPropertyClass::DoSetValue(%s)"),m_label.c_str());
    int intval = (int) value.GetLong();
    m_index = GetIndexForValue(intval);
}

wxPGVariant wxBaseEnumPropertyClass::DoGetValue () const
{
    if ( m_index < 0 )
        return wxPGVariant((long)-1);

    int val;
    GetEntry(m_index,&val);

    return val;
}

wxString wxBaseEnumPropertyClass::GetValueAsString ( int ) const
{
    if ( m_index >= 0 )
    {
        int unused_val;
        const wxString* pstr = GetEntry( m_index, &unused_val );

        if ( pstr )
            return *pstr;
    }
    return wxEmptyString;
}

bool wxBaseEnumPropertyClass::SetValueFromString ( const wxString& text, int WXUNUSED(arg_flags) )
{
    size_t i = 0;
    const wxString* entry_label;
    int entry_value;
    int use_index = -1;
    long use_value = 0;

    //wxLogDebug(wxT("property: %s"),m_name.c_str());
    //wxLogDebug(wxT("trying: %s"),text.c_str());

    entry_label = GetEntry(i,&entry_value);
    while ( entry_label )
    {
        //wxLogDebug(wxT("entry_label: %s"),entry_label->c_str());
        if ( text.CmpNoCase(*entry_label) == 0 )
        {
            use_index = (int)i;
            use_value = (long)entry_value;
            break;
        }

        i++;
        entry_label = GetEntry(i,&entry_value);
    }

    if ( m_index != use_index )
    {
        if ( use_index != -1 )
            // FIXME: Why can't this be virtual call?
            wxBaseEnumPropertyClass::DoSetValue ( use_value );
        else
            m_index = -1;

        return true;
    }
    /*}
    else if ( arg_flags & wxPG_REPORT_ERROR )
    {
        wxString s;
        s.Printf ( wxT("\"%s\" was not among valid choices."), text.c_str() );
        ShowError(s);
    }*/
    return false;
}

bool wxBaseEnumPropertyClass::SetValueFromInt ( long value, int arg_flags )
{
    if ( arg_flags & wxPG_FULL_VALUE )
    {
        DoSetValue(value);
        return true;
    }
    else
    {
        if ( m_index != value )
        {
            m_index = value;
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------
// wxEnumProperty
// -----------------------------------------------------------------------

// Class body is in propdev.h.

wxPGProperty* wxEnumProperty ( const wxString& label, const wxString& name, const wxChar** labels,
    const long* values, int value )
{
    return new wxEnumPropertyClass (label,name,labels,values,value);
}

wxPGProperty* wxEnumProperty( const wxString& label, const wxString& name,
    const wxArrayString& labels, const wxArrayInt& values, int value )
{
    return new wxEnumPropertyClass (label,name,labels,values,value);
}

wxPGProperty* wxEnumProperty( const wxString& label, const wxString& name,
    const wxArrayString& labels, int value )
{
    return new wxEnumPropertyClass (label,name,labels,*((const wxArrayInt*)NULL),value);
}

wxPGProperty* wxEnumProperty ( const wxString& label, const wxString& name, 
    wxPGChoices& choices, int value )
{
    return new wxEnumPropertyClass (label,name,choices,value);
}

WX_PG_IMPLEMENT_CLASSINFO(wxEnumProperty,wxBasePropertyClass)

WX_PG_IMPLEMENT_PROPERTY_CLASS_PLAIN(wxEnumProperty,long,Choice)

wxEnumPropertyClass::wxEnumPropertyClass ( const wxString& label, const wxString& name, const wxChar** labels,
    const long* values, int value ) : wxBaseEnumPropertyClass(label,name)
{
    m_index = 0;

    if ( labels )
    {
        m_choices.Add(labels,values);

        if ( GetItemCount() )
            wxEnumPropertyClass::DoSetValue( value );
    }
}

wxEnumPropertyClass::wxEnumPropertyClass ( const wxString& label, const wxString& name, const wxChar** labels,
    const long* values, wxPGChoices* choicesCache, int value )
    : wxBaseEnumPropertyClass(label,name)
{
    m_index = 0;

    wxASSERT( choicesCache );

    if ( choicesCache->IsOk() )
    {
        m_choices.Assign( *choicesCache );
    }
    else if ( labels )
    {
        m_choices.Add(labels,values);

        if ( GetItemCount() )
            wxEnumPropertyClass::DoSetValue( value );
    }
}

wxEnumPropertyClass::wxEnumPropertyClass ( const wxString& label, const wxString& name,
    const wxArrayString& labels, const wxArrayInt& values, int value ) : wxBaseEnumPropertyClass(label,name)
{
    m_index = 0;

    if ( &labels )
    {
        wxPGChoices choices(labels,values);
        m_choices = choices.ExtractData();

        if ( GetItemCount() )
            wxEnumPropertyClass::DoSetValue( value );
    }
}

wxEnumPropertyClass::wxEnumPropertyClass ( const wxString& label, const wxString& name, 
    wxPGChoices& choices, int value )
    : wxBaseEnumPropertyClass(label,name)
{
    m_choices.Assign( choices );

    if ( GetItemCount() )
        wxEnumPropertyClass::DoSetValue( value );
}

int wxEnumPropertyClass::GetIndexForValue( int value ) const
{
    if ( !m_choices.IsOk() )
        return -1;

    const wxArrayInt& arrValues = m_choices.GetValues();

    if ( arrValues.GetCount() )
    {
        int intval = arrValues.Index(value);

        // TODO: Use real default instead of 0.
        if ( intval < 0 )
            intval = 0;

        return intval;
    }
    return value;
}

wxEnumPropertyClass::~wxEnumPropertyClass () 
{
    //wxPGUnRefChoices(m_choices);
}

const wxString* wxEnumPropertyClass::GetEntry( size_t index, int* pvalue ) const
{
    if ( m_choices.IsOk() && index < m_choices.GetCount() )
    {
        const wxArrayInt& arrValues = m_choices.GetValues();

        int value = (int)index;
        if ( arrValues.GetCount() )
            value = arrValues[index];

        *pvalue = value;

        return &m_choices.GetLabel(index);
    }
    return (const wxString*) NULL;
}

int wxEnumPropertyClass::GetChoiceInfo ( wxPGChoiceInfo* choiceinfo )
{
    if ( choiceinfo )
    {
        if ( !(m_flags & wxPG_PROP_STATIC_CHOICES) )
            choiceinfo->m_choices = &m_choices;

        if ( !m_choices.IsOk() )
            return -1;

        choiceinfo->m_itemCount = m_choices.GetCount();
        if ( m_choices.GetCount() )
            choiceinfo->m_arrWxString = (wxString*)&m_choices.GetLabel(0);
    }

    if ( !m_choices.IsOk() )
            return -1;

    return m_index;
}

// -----------------------------------------------------------------------
// wxFlagsProperty
// -----------------------------------------------------------------------

// Class body is in propdev.h.

wxPGProperty* wxFlagsProperty( const wxString& label, const wxString& name, const wxChar** labels,
    const long* values, int value )
{
    return new wxFlagsPropertyClass (label,name,labels,values,value);
}

wxPGProperty* wxFlagsProperty( const wxString& label, const wxString& name,
    const wxArrayString& labels, const wxArrayInt& values, int value )
{
    return new wxFlagsPropertyClass (label,name,labels,values,value);
}

wxPGProperty* wxFlagsProperty( const wxString& label, const wxString& name,
    const wxArrayString& labels, int value )
{
    return new wxFlagsPropertyClass (label,name,labels,*((const wxArrayInt*)NULL),value);
}

wxPGProperty* wxFlagsProperty( const wxString& label, const wxString& name, 
    wxPGChoices& constants, int value )
{
    return new wxFlagsPropertyClass (label,name,constants,value);
}

WX_PG_IMPLEMENT_CLASSINFO(wxFlagsProperty,wxBaseParentPropertyClass)

WX_PG_IMPLEMENT_PROPERTY_CLASS_PLAIN(wxFlagsProperty,long,TextCtrl)

void wxFlagsPropertyClass::Init ()
{
    long value = m_value;

    //
    // Generate children
    //
    unsigned int i;

    unsigned int prevChildCount = m_children.GetCount();

    int oldSel = -1;
    if ( prevChildCount )
    {
        wxPropertyGridState* state = GetParentState();

        // State safety check (it may be NULL in immediate parent)
        //wxPGPropertyWithChildren* parent = GetParent();
        //while ( !state ) { wxASSERT(parent); state = parent->GetParentState(); parent = parent->GetParent(); }
        wxASSERT( state );

        if ( state )
        {
            wxPGProperty* selected = state->GetSelection();
            if ( selected )
            {
                if ( selected->GetParent() == this )
                    oldSel = selected->GetArrIndex();
                else if ( selected == this )
                    oldSel = -2;
            }
        }
        state->ClearSelection();
    }

    // Delete old children
    for ( i=0; i<prevChildCount; i++ )
        delete ( (wxPGProperty*) m_children[i] );

    m_children.Empty();

    if ( m_choices.IsOk() )
    {
        const wxArrayInt& values = GetValues();

        for ( i=0; i<GetItemCount(); i++ )
        {
            bool child_val;
            if ( values.GetCount() )
                child_val = ( value & values[i] )?TRUE:FALSE;
            else
                child_val = ( value & (1<<i) )?TRUE:FALSE;

            wxPGProperty* bool_prop;

        #if wxUSE_INTL
            if ( wxPGGlobalVars->m_autoGetTranslation )
            {
                bool_prop = wxBoolProperty( ::wxGetTranslation ( GetLabel(i) ), wxEmptyString, child_val );
            }
            else
        #endif
            {
                bool_prop = wxBoolProperty( GetLabel(i), wxEmptyString, child_val );
            }
            AddChild(bool_prop);
        }

        m_oldChoicesData = m_choices.GetDataPtr();
    }

    if ( prevChildCount )
        SubPropsChanged(oldSel);
}

wxFlagsPropertyClass::wxFlagsPropertyClass ( const wxString& label, const wxString& name,
    const wxChar** labels, const long* values, long value ) : wxPGPropertyWithChildren(label,name)
{

    m_value = 0;
    m_oldChoicesData = (wxPGChoicesData*) NULL;

    if ( labels )
    {
        m_choices.Set(labels,values);

        wxASSERT ( GetItemCount() );

        DoSetValue( value );
    }
}

wxFlagsPropertyClass::wxFlagsPropertyClass ( const wxString& label, const wxString& name,
        const wxArrayString& labels, const wxArrayInt& values, int value )
    : wxPGPropertyWithChildren(label,name)
{

    m_value = 0;
    m_oldChoicesData = (wxPGChoicesData*) NULL;

    if ( &labels )
    {
        m_choices.Set(labels,values);

        wxASSERT ( GetItemCount() );

        DoSetValue( value );
    }
}

wxFlagsPropertyClass::wxFlagsPropertyClass ( const wxString& label, const wxString& name, 
    wxPGChoices& choices, long value )
    : wxPGPropertyWithChildren(label,name)
{
    m_oldChoicesData = (wxPGChoicesData*) NULL;

    m_choices.Assign(choices);

    wxASSERT ( GetItemCount() );

    DoSetValue( value );
}

wxFlagsPropertyClass::~wxFlagsPropertyClass () 
{
    //wxPGUnRefChoices(m_choices);
}

void wxFlagsPropertyClass::DoSetValue ( wxPGVariant value )
{
    if ( !m_choices.IsOk() || !GetItemCount() )
    {
        m_value = 0;
        return;
    }

    long val = value.GetLong();

    long full_flags = 0;

    // normalize the value (i.e. remove extra flags)
    unsigned int i;
    const wxArrayInt& values = GetValues();
    if ( values.GetCount() )
    {
        for ( i = 0; i < GetItemCount(); i++ )
            full_flags |= values[i];
    }
    else
    {
        for ( i = 0; i < GetItemCount(); i++ )
            full_flags |= (1<<i);
    }
    val &= full_flags;

    m_value = val;

    // Need to (re)init now?
    if ( GetCount() != GetItemCount() ||
         m_choices.GetDataPtr() != m_oldChoicesData )
    {
        Init();
    }

    RefreshChildren();
}

wxPGVariant wxFlagsPropertyClass::DoGetValue () const
{
    return wxPGVariant((long)m_value);
}

wxString wxFlagsPropertyClass::GetValueAsString ( int ) const
{
    wxString text;

    if ( !m_choices.IsOk() )
        return text;

    long flags = m_value;
    unsigned int i;
    const wxArrayInt& values = GetValues();

    if ( values.GetCount() )
    {
        for ( i = 0; i < GetItemCount(); i++ )
        {
            if ( flags & values[i] )
            {
                text += GetLabel(i);
                text += wxT(", ");
            }
        }
    }
    else
    {
        for ( i = 0; i < GetItemCount(); i++ )
            if ( flags & (1<<i) )
            {
                text += GetLabel(i);
                text += wxT(", ");
            }
    }

    // remove last comma
    if ( text.Len() > 1 )
        text.Truncate ( text.Len() - 2 );

    return text;
}

// Translate string into flag tokens
bool wxFlagsPropertyClass::SetValueFromString ( const wxString& text, int )
{
    if ( !m_choices.IsOk() || !GetItemCount() )
        return false;

    long new_flags = 0;

    // semicolons are no longer valid delimeters
    WX_PG_TOKENIZER1_BEGIN(text,wxT(','))

        if ( token.length() )
        {
            // Determine which one it is
            long bit = IdToBit( token );

            if ( bit != -1 )
            {
                // Changed?
                new_flags |= bit;
            }
            else
            {
            // Unknown identifier
                wxString s;
                s.Printf ( wxT("! %s: Unknown flag identifier \"%s\""), m_label.c_str(), token.c_str() );
                ShowError(s);
            }
        }
        
    WX_PG_TOKENIZER1_END()

    //wxLogDebug ( wxT("new_flags = 0x%X, old_flags = 0x%X"),new_flags,m_value);

    if ( new_flags != m_value )
    {
        // Set child modified states
        unsigned int i;
        const wxArrayInt& values = GetValues();
        if ( values.GetCount() )
            for ( i = 0; i < GetItemCount(); i++ )
            {
                long flag = values[i];
                if ( (new_flags & flag) != (m_value & flag) )
                    ((wxPGProperty*)m_children.Item( i ))->SetFlag ( wxPG_PROP_MODIFIED );
            }
        else
            for ( i = 0; i < GetItemCount(); i++ )
            {
                long flag = (1<<i);
                if ( (new_flags & flag) != (m_value & flag) )
                    ((wxPGProperty*)m_children.Item( i ))->SetFlag ( wxPG_PROP_MODIFIED );
            }

        DoSetValue ( new_flags );

        return TRUE;
    }

    return FALSE;
}

// Converts string id to a relevant bit.
long wxFlagsPropertyClass::IdToBit ( const wxString& id ) const
{
    unsigned int i;
    const wxArrayInt& values = GetValues();
    for ( i = 0; i < GetItemCount(); i++ )
    {
        const wxChar* ptr = GetLabel(i);
        if ( id == ptr
             /*wxStrncmp(id,ptr,id_len) == 0 &&
             ptr[id_len] == 0*/
           )
        {
            //*pindex = i;
            if ( values.GetCount() )
                return values[i];
            return (1<<i);
        }
    }
    return -1;
}

void wxFlagsPropertyClass::RefreshChildren()
{
    if ( !m_choices.IsOk() || !GetCount() ) return;
    const wxArrayInt& values = GetValues();
    long flags = m_value;
    unsigned int i;
    if ( values.GetCount() )
        for ( i = 0; i < GetItemCount(); i++ )
            Item(i)->DoSetValue ( ((long)((flags & values[i])?TRUE:FALSE)) );
    else
        for ( i = 0; i < GetItemCount(); i++ )
            Item(i)->DoSetValue ( ((long)((flags & (1<<i))?TRUE:FALSE)) );
}

void wxFlagsPropertyClass::ChildChanged ( wxPGProperty* p )
{
    wxASSERT ( this == p->GetParent() );

    const wxArrayInt& values = GetValues();
    long val = p->DoGetValue().GetRawLong(); // bypass type checking
    unsigned int iip = p->GetIndexInParent();
    unsigned long vi = (1<<iip);
    if ( values.GetCount() ) vi = values[iip];
    if ( val )
        m_value |= vi;
    else
        m_value &= ~(vi);
}

int wxFlagsPropertyClass::GetChoiceInfo ( wxPGChoiceInfo* choiceinfo )
{
    if ( choiceinfo )
        choiceinfo->m_choices = &m_choices;
    return -1;
}

// -----------------------------------------------------------------------
// wxDirProperty
// -----------------------------------------------------------------------

WX_PG_IMPLEMENT_STRING_PROPERTY_WITH_VALIDATOR(wxDirProperty)

#if wxUSE_VALIDATORS

wxValidator* wxDirPropertyClass::DoGetValidator() const
{
    return wxFilePropertyClass::GetClassValidator();
}

#endif

bool wxDirPropertyClass::OnButtonClick ( wxPropertyGrid* propgrid, wxString& value )
{
    wxSize dlg_sz(300,400);

    wxDirDialog dlg(propgrid,_("Choose a directory:"),
        value,0,
        propgrid->GetGoodEditorDialogPosition (this,dlg_sz),dlg_sz );

    if ( dlg.ShowModal() == wxID_OK )
    {
        value = dlg.GetPath();
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------
// wxFileProperty
// -----------------------------------------------------------------------

// Class body is in propdev.h.

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxFileProperty,wxBaseProperty,
                               wxString,const wxString&,TextCtrlAndButton)

wxFilePropertyClass::wxFilePropertyClass ( const wxString& label, const wxString& name,
    const wxString& value ) : wxPGProperty(label,name)
{
    m_wildcard = _("All files (*.*)|*.*");
    m_flags |= wxPG_PROP_SHOW_FULL_FILENAME;
    m_indFilter = -1;

    DoSetValue(value);
}

wxFilePropertyClass::~wxFilePropertyClass () {}

#if wxUSE_VALIDATORS

wxValidator* wxFilePropertyClass::GetClassValidator ()
{
    WX_PG_DOGETVALIDATOR_ENTRY()

    wxTextValidator* validator =  new wxTextValidator(wxFILTER_EXCLUDE_CHAR_LIST);

    wxArrayString exChars;
    exChars.Add(wxT("?"));
    exChars.Add(wxT("*"));
    exChars.Add(wxT("|"));
    exChars.Add(wxT("<"));
    exChars.Add(wxT(">"));
    exChars.Add(wxT("\""));

    validator->SetExcludes(exChars);

    WX_PG_DOGETVALIDATOR_EXIT(validator)
}

wxValidator* wxFilePropertyClass::DoGetValidator () const
{
    return GetClassValidator();
}

#endif

void wxFilePropertyClass::DoSetValue ( wxPGVariant value )
{
    const wxString& str = wxPGVariantToString(value);
    m_filename = str;
    m_fnstr = str;

    // Find index for extension.
    if ( m_indFilter < 0 && m_fnstr.length() )
    {
        wxChar a = wxT(' ');
        const wxChar* p = m_wildcard.c_str();
        wxString ext = m_filename.GetExt();
        int curind = 0;
        do
        {
            while ( a && a != '|' ) { a = *p; p++; }
            if ( !a ) break;

            a = *p;
            p++;
            if ( !a ) break;
            a = *p;
            p++;

            const wxChar* ext_begin = p;

            if ( *ext_begin == wxT('*') )
            {
                m_indFilter = curind;
                break;
            }

            while ( a && a != '|' ) { a = *p; p++; }

            a = wxT(' ');

            int count = p-ext_begin-1;
            if ( count > 0 )
            {
                wxASSERT ( count < 32 );
                wxString found_ext = m_wildcard.Mid(ext_begin-m_wildcard.c_str(),count);

                if ( ext.CmpNoCase(found_ext) == 0 )
                {
                    m_indFilter = curind;
                    break;
                }
            }

            curind++;

        } while ( a );
    }
}

wxPGVariant wxFilePropertyClass::DoGetValue () const
{
    return wxPGVariant(m_fnstr);
}

wxString wxFilePropertyClass::GetValueAsString ( int arg_flags ) const
{
    if ( (m_flags & wxPG_PROP_SHOW_FULL_FILENAME) || (arg_flags & wxPG_FULL_VALUE) )
        return m_filename.GetFullPath();
    else
        return m_filename.GetFullName();
}

bool wxFilePropertyClass::OnEvent ( wxPropertyGrid* propgrid, wxWindow* primary,
    wxEvent& event )
{
    if ( event.GetEventType() == wxEVT_COMMAND_BUTTON_CLICKED )
    {
        // If text in control is changed, then update it to value.
        PrepareValueForDialogEditing(propgrid);

        wxString path;
        path = m_filename.GetPath();

        wxFileDialog dlg(propgrid,_("Choose a file"),m_filename.GetPath(),
            wxEmptyString, m_wildcard, 0, wxDefaultPosition );

        if ( m_indFilter >= 0 )
            dlg.SetFilterIndex( m_indFilter );

        if ( dlg.ShowModal() == wxID_OK )
        {
            m_indFilter = dlg.GetFilterIndex();
            wxString path = dlg.GetPath();
            SetValueFromString ( path, wxPG_FULL_VALUE );
            if ( primary )
                GetEditorClass()->SetControlStringValue( primary, GetValueAsString(0) );
            return true;
        }
    }
    return false;
}

bool wxFilePropertyClass::SetValueFromString ( const wxString& text, int arg_flags )
{
    if ( (m_flags & wxPG_PROP_SHOW_FULL_FILENAME) || (arg_flags & wxPG_FULL_VALUE) )
    {
        if ( m_filename != text )
        {
            return StdValidationProcedure( text );
        }
    }
    else
    {
        if ( m_filename.GetFullName() != text )
        {
            wxFileName fn = m_filename;
            fn.SetFullName(text);
            wxString val = fn.GetFullPath();
            return StdValidationProcedure( val );
        }
    }

    return false;
}

void wxFilePropertyClass::SetAttribute ( int id, wxVariant& value )
{
    if ( id == wxPG_FILE_SHOW_FULL_PATH )
    {
        if ( value.GetLong() )
            m_flags |= wxPG_PROP_SHOW_FULL_FILENAME;
        else
            m_flags &= ~(wxPG_PROP_SHOW_FULL_FILENAME);
    }
    else if ( id == wxPG_FILE_WILDCARD )
    {
        m_wildcard = value.GetString();
    }
}

// -----------------------------------------------------------------------
// wxLongStringProperty
// -----------------------------------------------------------------------

// Class body is in propdev.h.

/*
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxLongStringProperty,wxStringProperty,
                               wxString,const wxString&,TextCtrlAndButton)

wxLongStringPropertyClass::wxLongStringPropertyClass ( const wxString& label, const wxString& name,
    const wxString& value ) : wxStringPropertyClass(label,name,value)
{
}

wxLongStringPropertyClass::~wxLongStringPropertyClass () { }

*/

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxLongStringProperty,wxBaseProperty,
                               wxString,const wxString&,TextCtrlAndButton)

wxLongStringPropertyClass::wxLongStringPropertyClass ( const wxString& label, const wxString& name,
    const wxString& value ) : wxBasePropertyClass(label,name)
{
    DoSetValue(value);
}

wxLongStringPropertyClass::~wxLongStringPropertyClass () {}

void wxLongStringPropertyClass::DoSetValue ( wxPGVariant value )
{
    m_value = wxPGVariantToString(value);
}

wxPGVariant wxLongStringPropertyClass::DoGetValue () const
{
    return wxPGVariant(m_value);
}

wxString wxLongStringPropertyClass::GetValueAsString ( int ) const
{
    return m_value;
}

bool wxLongStringPropertyClass::OnEvent ( wxPropertyGrid* propgrid, wxWindow* primary,
    wxEvent& event )
{
    if ( event.GetEventType() == wxEVT_COMMAND_BUTTON_CLICKED )
    {
        // Update the value
        PrepareValueForDialogEditing(propgrid);

        wxString val1 = GetValueAsString(0);
        wxString val_orig = val1;

        wxString value;
        wxPropertyGrid::ExpandEscapeSequences(value,val1);

        // Run editor dialog.
        if ( OnButtonClick(propgrid,value) )
        {
            wxPropertyGrid::CreateEscapeSequences(val1,value);
            if ( val1 != val_orig )
            {
                SetValueFromString ( val1, 0 );
                UpdateControl ( primary );
                return true;
            }
        }
    }
    return false;
}

bool wxLongStringPropertyClass::OnButtonClick ( wxPropertyGrid* propgrid, wxString& value )
{
    // launch editor dialog
    wxDialog* dlg = new wxDialog (propgrid,-1,m_label,
        wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxCLIP_CHILDREN);

    // Multi-line text editor dialog.
    const int spacing = 8;
    wxBoxSizer* topsizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer* rowsizer = new wxBoxSizer( wxHORIZONTAL );
    wxTextCtrl* ed = new wxTextCtrl(dlg,11,value,
        wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE);
    rowsizer->Add ( ed, 1, wxEXPAND|wxALL, spacing );
    topsizer->Add ( rowsizer, 1, wxEXPAND, 0 );
    rowsizer = new wxBoxSizer( wxHORIZONTAL );
    const int but_sz_flags =
        wxALIGN_RIGHT|wxALIGN_CENTRE_VERTICAL|wxBOTTOM|wxLEFT|wxRIGHT;
    rowsizer->Add ( new wxButton(dlg,wxID_OK,_("Ok")),
        0, but_sz_flags, spacing );
    rowsizer->Add ( new wxButton(dlg,wxID_CANCEL,_("Cancel")),
        0, but_sz_flags, spacing );
    topsizer->Add ( rowsizer, 0, wxALIGN_RIGHT|wxALIGN_CENTRE_VERTICAL, 0 );

    dlg->SetSizer ( topsizer );
    topsizer->SetSizeHints( dlg );

    dlg->SetSize (400,300);

    dlg->Move ( propgrid->GetGoodEditorDialogPosition (this,dlg->GetSize()) );

    int res = dlg->ShowModal();

    if ( res == wxID_OK )
    {
        value = ed->GetValue();
        dlg->Destroy();
        return true;
    }
    dlg->Destroy();
    return false;
}

bool wxLongStringPropertyClass::SetValueFromString ( const wxString& text, int )
{
    if ( m_value != text )
    {
        DoSetValue ( text );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------
// wxArrayEditorDialog
// -----------------------------------------------------------------------

BEGIN_EVENT_TABLE(wxArrayEditorDialog, wxDialog)
    EVT_IDLE(wxArrayEditorDialog::OnIdle)
    EVT_LISTBOX(24, wxArrayEditorDialog::OnListBoxClick)
    EVT_LISTBOX(24, wxArrayEditorDialog::OnListBoxClick)
    EVT_TEXT_ENTER(21, wxArrayEditorDialog::OnAddClick)
    EVT_BUTTON(22, wxArrayEditorDialog::OnAddClick)
    EVT_BUTTON(23, wxArrayEditorDialog::OnDeleteClick)
    EVT_BUTTON(25, wxArrayEditorDialog::OnUpClick)
    EVT_BUTTON(26, wxArrayEditorDialog::OnDownClick)
    EVT_BUTTON(27, wxArrayEditorDialog::OnUpdateClick)
    //EVT_BUTTON(28, wxArrayEditorDialog::OnCustomEditClick)
END_EVENT_TABLE()

IMPLEMENT_ABSTRACT_CLASS(wxArrayEditorDialog, wxDialog)

#include <wx/statline.h>

// -----------------------------------------------------------------------

void wxArrayEditorDialog::OnIdle (wxIdleEvent&)
{
    //
    // Do control focus detection here.
    //

    if ( m_curFocus == 0 && FindFocus() == m_lbStrings )
    {
        // ListBox was just focused.
        m_butAdd->Enable(false);
        m_butUpdate->Enable(false);
        m_butRemove->Enable(true);
        m_butUp->Enable(true);
        m_butDown->Enable(true);
        m_curFocus = 1;
    }
    else if ( (m_curFocus == 1 && FindFocus() == m_edValue) /*|| m_curFocus == 2*/ )
    {
        // TextCtrl was just focused.
        m_butAdd->Enable(true);
        bool upd_enable = false;
        if ( m_lbStrings->GetCount() && m_lbStrings->GetSelection() >= 0 )
            upd_enable = true;
        m_butUpdate->Enable(upd_enable);
        m_butRemove->Enable(false);
        m_butUp->Enable(false);
        m_butDown->Enable(false);
        m_curFocus = 0;
    }
    //wxLogDebug(wxT("wxArrayEditorDialog::OnFocusChange"));
}

// -----------------------------------------------------------------------

wxArrayEditorDialog::wxArrayEditorDialog()
    : wxDialog()
{
    Init();
}

// -----------------------------------------------------------------------

void wxArrayEditorDialog::Init()
{
    m_custBtText = (const wxChar*) NULL;
    //m_pCallingClass = (wxArrayStringPropertyClass*) NULL;
}

// -----------------------------------------------------------------------

wxArrayEditorDialog::wxArrayEditorDialog(wxWindow *parent,
                              const wxString& message,
                              const wxString& caption,
                              long style,
                              const wxPoint& pos,
                              const wxSize& sz )
                              : wxDialog()
{
    Init();
    Create(parent,message,caption,style,pos,sz);
}

// -----------------------------------------------------------------------

bool wxArrayEditorDialog::Create(wxWindow *parent,
                const wxString& message,
                const wxString& caption,
                long style,
                const wxPoint& pos,
                const wxSize& sz )
{

    bool res = wxDialog::Create (parent,1,caption,pos,sz,style);

    const int spacing = 4;

    m_modified = false;

    m_curFocus = 1;

    const int but_sz_flags =
        wxALIGN_RIGHT|wxALIGN_CENTRE_VERTICAL|wxALL; //wxBOTTOM|wxLEFT|wxRIGHT;

    wxBoxSizer* topsizer = new wxBoxSizer( wxVERTICAL );

    // Message
    if ( message.length() )
        topsizer->Add ( new wxStaticText(this,-1,message),
            0, wxALIGN_LEFT|wxALIGN_CENTRE_VERTICAL|wxALL, spacing );

    // String editor
    wxBoxSizer* rowsizer = new wxBoxSizer( wxHORIZONTAL );
    /*rowsizer->Add ( new wxStaticText(this,-1,_("String:")),
        0, wxALIGN_LEFT|wxALIGN_CENTRE_VERTICAL|wxBOTTOM|wxTOP|wxLEFT, spacing );*/
    m_edValue = new wxTextCtrl(this,21,wxEmptyString,
        wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);
    rowsizer->Add ( m_edValue,
        1, wxALIGN_LEFT|wxALIGN_CENTRE_VERTICAL|wxALL, spacing );

    // Add button
    m_butAdd = new wxButton(this,22,_("Add"));
    rowsizer->Add ( m_butAdd,
        0, wxALIGN_LEFT|wxALIGN_CENTRE_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, spacing );
    topsizer->Add ( rowsizer, 0, wxEXPAND, spacing );

    // Separator line
    topsizer->Add ( new wxStaticLine(this,-1),
        0, wxEXPAND|wxBOTTOM|wxLEFT|wxRIGHT, spacing );

    rowsizer = new wxBoxSizer( wxHORIZONTAL );

    // list box
    //m_lbStrings = new wxListBox(this, 24, wxDefaultPosition, wxDefaultSize, array);
    m_lbStrings = new wxListBox(this, 24, wxDefaultPosition, wxDefaultSize);
    unsigned int i;
    for ( i=0; i<ArrayGetCount(); i++ )
        m_lbStrings->Append( ArrayGet(i) );
    rowsizer->Add ( m_lbStrings, 1, wxEXPAND|wxRIGHT, spacing );

    // Manipulator buttons
    wxBoxSizer* colsizer = new wxBoxSizer( wxVERTICAL );
    m_butCustom = (wxButton*) NULL;
    if ( m_custBtText )
    {
        m_butCustom = new wxButton(this,28,::wxGetTranslation(m_custBtText));
        colsizer->Add ( m_butCustom,
            0, wxALIGN_CENTER|wxTOP/*wxALIGN_LEFT|wxALIGN_CENTRE_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT*/,
            spacing );
    }
    m_butUpdate = new wxButton(this,27,_("Update"));
    colsizer->Add ( m_butUpdate,
        0, wxALIGN_CENTER|wxTOP, spacing );
    m_butRemove = new wxButton(this,23,_("Remove"));
    colsizer->Add ( m_butRemove,
        0, wxALIGN_CENTER|wxTOP, spacing );
    m_butUp = new wxButton(this,25,_("Up"));
    colsizer->Add ( m_butUp,
        0, wxALIGN_CENTER|wxTOP, spacing );
    m_butDown = new wxButton(this,26,_("Down"));
    colsizer->Add ( m_butDown,
        0, wxALIGN_CENTER|wxTOP, spacing );
    rowsizer->Add ( colsizer, 0, 0, spacing );

    topsizer->Add ( rowsizer, 1, wxLEFT|wxRIGHT|wxEXPAND, spacing );

    // Separator line
    topsizer->Add ( new wxStaticLine(this,-1),
        0, wxEXPAND|wxTOP|wxLEFT|wxRIGHT, spacing );

    // buttons
    rowsizer = new wxBoxSizer( wxHORIZONTAL );
    /*
    const int but_sz_flags =
        wxALIGN_RIGHT|wxALIGN_CENTRE_VERTICAL|wxBOTTOM|wxLEFT|wxRIGHT;
    */
    rowsizer->Add ( new wxButton(this,wxID_OK,_("Ok")),
        0, but_sz_flags, spacing );
    rowsizer->Add ( new wxButton(this,wxID_CANCEL,_("Cancel")),
        0, but_sz_flags, spacing );
    topsizer->Add ( rowsizer, 0, wxALIGN_RIGHT|wxALIGN_CENTRE_VERTICAL, 0 );

    m_edValue->SetFocus();

    SetSizer ( topsizer );
    topsizer->SetSizeHints( this );

    if ( sz.x == wxDefaultSize.x &&
         sz.y == wxDefaultSize.y
       )
        SetSize ( wxSize(275,360) );
    else
        SetSize (sz);

    return res;
}

// -----------------------------------------------------------------------

void wxArrayEditorDialog::OnAddClick(wxCommandEvent& )
{
    wxString text = m_edValue->GetValue();
    if ( text.length() )
    {
        if ( ArrayInsert( text, -1 ) )
        {
            m_lbStrings->Append( text );
            m_modified = true;
            m_edValue->Clear();
        }
    }
}

// -----------------------------------------------------------------------

void wxArrayEditorDialog::OnDeleteClick(wxCommandEvent& )
{
    int index = m_lbStrings->GetSelection();
    if ( index >= 0 )
    {
        ArrayRemoveAt( index );
        m_lbStrings->Delete ( index );
        m_modified = true;
    }
}

// -----------------------------------------------------------------------

void wxArrayEditorDialog::OnUpClick(wxCommandEvent& )
{
    int index = m_lbStrings->GetSelection();
    if ( index > 0 )
    {
        ArraySwap(index-1,index);
        /*wxString old_str = m_array[index-1];
        wxString new_str = m_array[index];
        m_array[index-1] = new_str;
        m_array[index] = old_str;*/
        m_lbStrings->SetString ( index-1, ArrayGet(index-1) );
        m_lbStrings->SetString ( index, ArrayGet(index) );
        m_lbStrings->SetSelection ( index-1 );
        m_modified = true;
    }
}

// -----------------------------------------------------------------------

void wxArrayEditorDialog::OnDownClick(wxCommandEvent& )
{
    int index = m_lbStrings->GetSelection();
    if ( index >= 0 && index < (m_lbStrings->GetCount()-1) )
    {
        ArraySwap(index,index+1);
        /*wxString old_str = m_array[index+1];
        wxString new_str = m_array[index];
        m_array[index+1] = new_str;
        m_array[index] = old_str;*/
        m_lbStrings->SetString ( index+1, ArrayGet(index+1) );
        m_lbStrings->SetString ( index, ArrayGet(index) );
        m_lbStrings->SetSelection ( index+1 );
        m_modified = true;
    }
}

// -----------------------------------------------------------------------

void wxArrayEditorDialog::OnUpdateClick(wxCommandEvent& )
{
    int index = m_lbStrings->GetSelection();
    if ( index >= 0 )
    {
        wxString str = m_edValue->GetValue();
        if ( ArraySet(index,str) )
        {
            m_lbStrings->SetString ( index, str );
            //m_array[index] = str;
            m_modified = true;
        }
    }
}

// -----------------------------------------------------------------------

/*void wxArrayEditorDialog::OnCustomEditClick(wxCommandEvent& )
{
    wxASSERT ( m_pCallingClass );
    wxString str = m_edValue->GetValue();
    if ( m_pCallingClass->OnCustomStringEdit(m_parent,str) )
    {
        //m_edValue->SetValue ( str );
        if ( ArrayInsert(-1,str) )
        {
            m_lbStrings->Append ( str );
            m_modified = true;
        }
    }
}*/

// -----------------------------------------------------------------------

void wxArrayEditorDialog::OnListBoxClick(wxCommandEvent& )
{
    int index = m_lbStrings->GetSelection();
    if ( index >= 0 )
    {
        m_edValue->SetValue ( m_lbStrings->GetString(index) );
    }
}

// -----------------------------------------------------------------------
// wxArrayStringEditorDialog
// -----------------------------------------------------------------------

class wxArrayStringEditorDialog : public wxArrayEditorDialog
{
public:
    wxArrayStringEditorDialog();

    void Init();

    wxArrayStringEditorDialog(wxWindow *parent,
                              const wxString& message,
                              const wxString& caption,
                              wxArrayString& array,
                              long style = wxAEDIALOG_STYLE,
                              const wxPoint& pos = wxDefaultPosition,
                              const wxSize& sz = wxDefaultSize );

    bool Create(wxWindow *parent,
                const wxString& message,
                const wxString& caption,
                wxArrayString& array,
                long style = wxAEDIALOG_STYLE,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& sz = wxDefaultSize );

    inline void SetCustomButton ( const wxChar* custBtText, wxArrayStringPropertyClass* pcc )
    {
        m_custBtText = custBtText;
        m_pCallingClass = pcc;
    }

    const wxArrayString& GetArray() const { return m_array; }

    void OnCustomEditClick(wxCommandEvent& event);

protected:
    wxArrayString   m_array;

    wxArrayStringPropertyClass*     m_pCallingClass;

    virtual wxString ArrayGet( size_t index );
    virtual size_t ArrayGetCount();
    virtual bool ArrayInsert( const wxString& str, int index );
    virtual bool ArraySet( size_t index, const wxString& str );
    virtual void ArrayRemoveAt( int index );
    virtual void ArraySwap( size_t first, size_t second );

private:
    DECLARE_DYNAMIC_CLASS_NO_COPY(wxArrayStringEditorDialog)
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(wxArrayStringEditorDialog, wxArrayEditorDialog)
    EVT_BUTTON(28, wxArrayStringEditorDialog::OnCustomEditClick)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(wxArrayStringEditorDialog, wxArrayEditorDialog)

// -----------------------------------------------------------------------

wxString wxArrayStringEditorDialog::ArrayGet( size_t index )
{
    return m_array[index];
}

size_t wxArrayStringEditorDialog::ArrayGetCount()
{
    return m_array.GetCount();
}

bool wxArrayStringEditorDialog::ArrayInsert( const wxString& str, int index )
{
    if (index<0)
        m_array.Add(str);
    else
        m_array.Insert(str,index);
    return true;
}

bool wxArrayStringEditorDialog::ArraySet( size_t index, const wxString& str )
{
    m_array[index] = str;
    return true;
}

void wxArrayStringEditorDialog::ArrayRemoveAt( int index )
{
    m_array.RemoveAt(index);
}

void wxArrayStringEditorDialog::ArraySwap( size_t first, size_t second )
{
    wxString old_str = m_array[first];
    wxString new_str = m_array[second];
    m_array[first] = new_str;
    m_array[second] = old_str;
}

wxArrayStringEditorDialog::wxArrayStringEditorDialog()
    : wxArrayEditorDialog()
{
    Init();
}

void wxArrayStringEditorDialog::Init()
{
    wxArrayEditorDialog::Init();
    m_pCallingClass = (wxArrayStringPropertyClass*) NULL;
}

wxArrayStringEditorDialog::wxArrayStringEditorDialog(wxWindow *parent,
                              const wxString& message,
                              const wxString& caption,
                              wxArrayString& array,
                              long style,
                              const wxPoint& pos,
                              const wxSize& sz )
                              : wxArrayEditorDialog()
{
    Init();
    Create(parent,message,caption,array,style,pos,sz);
/*#if defined(__WXGTK__) && !defined(__WXGTK20__)
    SetFont(*wxNORMAL_FONT);
#endif*/
}

bool wxArrayStringEditorDialog::Create(wxWindow *parent,
                const wxString& message,
                const wxString& caption,
                wxArrayString& array,
                long style,
                const wxPoint& pos,
                const wxSize& sz )
{

    m_array = array;

    return wxArrayEditorDialog::Create (parent,message,caption,style,pos,sz);
}

void wxArrayStringEditorDialog::OnCustomEditClick(wxCommandEvent& )
{
    wxASSERT ( m_pCallingClass );
    wxString str = m_edValue->GetValue();
    if ( m_pCallingClass->OnCustomStringEdit(m_parent,str) )
    {
        //m_edValue->SetValue ( str );
        m_lbStrings->Append ( str );
        m_array.Add ( str );
        m_modified = true;
    }
}

// -----------------------------------------------------------------------
// wxArrayStringProperty
// -----------------------------------------------------------------------

// Class body is in propdev.h

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxArrayStringProperty,wxBaseProperty,
                               wxArrayString,const wxArrayString&,TextCtrlAndButton)

wxArrayStringPropertyClass::wxArrayStringPropertyClass ( const wxString& label, const wxString& name,
    const wxArrayString& array ) : wxPGProperty(label,name)
{
    DoSetValue( array );
}

wxArrayStringPropertyClass::~wxArrayStringPropertyClass () { }

void wxArrayStringPropertyClass::DoSetValue ( wxPGVariant value )
{
    m_value = wxPGVariantToArrayString(value);
    GenerateValueAsString();
}

wxPGVariant wxArrayStringPropertyClass::DoGetValue () const
{
    return wxPGVariant(m_value);
}

wxString wxArrayStringPropertyClass::GetValueAsString ( int /*arg_flags*/ ) const
{
    //if ( !(arg_flags & wxPG_FULL_VALUE) )
        return m_display;

    /*wxString s_ret;
    wxPropertyGrid::ArrayStringToString(s_ret,m_value,wxT('"'),wxT('"'),0);
    return s_ret;*/
}

// Converts wxArrayString to a string separated by delimeters and spaces.
// preDelim is useful for "str1" "str2" style. Set flags to 1 to do slash
// conversion.
void wxPropertyGrid::ArrayStringToString ( wxString& dst, const wxArrayString& src,
                                           wxChar preDelim, wxChar postDelim,
                                           int flags )
{
    wxString pdr;

    unsigned int i;
    unsigned int itemcount = src.GetCount();

    wxChar preas[2];

    dst.Empty();

    if ( !preDelim )
        preas[0] = 0;
    else if ( (flags & 1) )
    {
        preas[0] = preDelim;
        preas[1] = 0;
        pdr = wxT("\\");
        pdr += preDelim;
    }

    if ( itemcount )
        dst.append ( preas );

    wxASSERT ( postDelim );

    for ( i = 0; i < itemcount; i++ )
    {
        wxString str( src.Item(i) );

        // Do some character conversion.
        // Convertes \ to \\ and <preDelim> to \<preDelim>
        // Useful when preDelim and postDelim are "\"".
        if ( flags & 1 )
        {
            str.Replace ( wxT("\\"), wxT("\\\\"), true );
            if ( pdr.length() )
                str.Replace ( preas, pdr, true );
        }

        dst.append ( str );

        if ( i < (itemcount-1) )
        {
            dst.append ( postDelim );
            dst.append ( wxT(" ") );
            dst.append ( preas );
        }
        else if ( preDelim )
            dst.append ( postDelim );
    }
}

void wxArrayStringPropertyClass::GenerateValueAsString ()
{
    wxPropertyGrid::ArrayStringToString(m_display,m_value,wxT('"'),wxT('"'),1);
}

// Default implementation doesn't do anything.
bool wxArrayStringPropertyClass::OnCustomStringEdit ( wxWindow*, wxString& )
{
    return false;
}

bool wxArrayStringPropertyClass::OnButtonClick ( wxPropertyGrid* propgrid, wxWindow* primary,
    const wxChar* cbt )
{
    // Update the value
    PrepareValueForDialogEditing(propgrid);

    // Create editor dialog.
    wxArrayStringEditorDialog dlg;

    if ( cbt )
        dlg.SetCustomButton(cbt,this);

    dlg.Create(propgrid, wxEmptyString, m_label, m_value );

    dlg.Move ( propgrid->GetGoodEditorDialogPosition (this,dlg.GetSize()) );

    int res = dlg.ShowModal();
    if ( res == wxID_OK && dlg.IsModified() )
    {
        DoSetValue ( dlg.GetArray() );
        UpdateControl ( primary );
        return true;
    }
    return false;
}

bool wxArrayStringPropertyClass::OnEvent ( wxPropertyGrid* propgrid, wxWindow* primary,
    wxEvent& event )
{
    if ( event.GetEventType() == wxEVT_COMMAND_BUTTON_CLICKED )
        return OnButtonClick(propgrid,primary,(const wxChar*) NULL);
    return false;
}

bool wxArrayStringPropertyClass::SetValueFromString ( const wxString& text, int )
{
    m_value.Empty();

    WX_PG_TOKENIZER2_BEGIN(text,wxT('"'))
        
        // Need to replace backslashes with empty characters
        // (opposite what is done in GenerateValueString).
        token.Replace ( wxT("\\"), wxT(""), true );

        m_value.Add ( token );

    WX_PG_TOKENIZER2_END()

    GenerateValueAsString();

    return true;
}

// -----------------------------------------------------------------------
// wxCustomProperty
// -----------------------------------------------------------------------

wxPGProperty* wxCustomProperty( const wxString& label, const wxString& name )
{
    return new wxCustomPropertyClass (label,name);
}

WX_PG_IMPLEMENT_CLASSINFO(wxCustomProperty,wxBaseParentPropertyClass)
wxPG_GETCLASSNAME_IMPLEMENTATION(wxCustomProperty)

const wxPGValueType* wxCustomPropertyClass::GetValueType() const
{
    return wxPG_VALUETYPE(wxString);
}

const wxPGEditor* wxCustomPropertyClass::DoGetEditorClass() const
{
    return wxPG_EDITOR(TextCtrl);
}

wxCustomPropertyClass::wxCustomPropertyClass(const wxString& label,
                                             const wxString& name)
    : wxPGPropertyWithChildren(label,name)
{
    m_parentingType = -2;
#ifdef wxPG_COMPATIBILITY_1_0_0
    m_callback = (wxPropertyGridCallback) NULL;
#endif
    //m_choices = &wxPGGlobalVars->m_emptyChoicesData;
    m_paintCallback = (wxPGPaintCallback) NULL;
}

wxCustomPropertyClass::~wxCustomPropertyClass()
{
    //wxPGUnRefChoices(m_choices);
}

void wxCustomPropertyClass::DoSetValue ( wxPGVariant value )
{
    m_value = value.GetString();
}

wxPGVariant wxCustomPropertyClass::DoGetValue () const
{
    return m_value;
}

bool wxCustomPropertyClass::SetValueFromString ( const wxString& text, int /*flags*/ )
{
    if ( text != m_value )
    {
        m_value = text;
        return true;
    }
    return false;
}

wxString wxCustomPropertyClass::GetValueAsString ( int /*arg_flags*/ ) const
{
    return m_value;
}

// Need to do some extra event handling.
#ifdef wxPG_COMPATIBILITY_1_0_0
bool wxCustomPropertyClass::OnEvent ( wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event )
{
    if ( event.GetEventType() == wxEVT_COMMAND_BUTTON_CLICKED )
    {
        if ( m_callback )
            return m_callback(propgrid,this,primary,0);
    }
    return false;
}

#endif

wxSize wxCustomPropertyClass::GetImageSize() const
{
    if ( m_paintCallback )
        return wxSize(-wxPG_CUSTOM_IMAGE_WIDTH,-wxPG_CUSTOM_IMAGE_WIDTH);

    return wxPGPropertyWithChildren::GetImageSize();
}

void wxCustomPropertyClass::OnCustomPaint( wxDC& dc,
                                           const wxRect& rect,
                                           wxPGPaintData& paintData )
{
    if ( m_paintCallback )
        m_paintCallback(this,dc,rect,paintData);
    else
        wxPGPropertyWithChildren::OnCustomPaint(dc,rect,paintData);
}

bool wxCustomPropertyClass::SetValueFromInt ( long value, int )
{
    size_t index = value;
    const wxArrayInt& values = m_choices.GetValues();
    if ( values.GetCount() )
        index = values.Index(value);

    const wxString& sAtIndex = m_choices.GetLabel(index);
    if ( sAtIndex != m_value )
    {
        m_value = sAtIndex;
        return true;
    }

    return false;
}

int wxCustomPropertyClass::GetChoiceInfo ( wxPGChoiceInfo* choiceinfo )
{
    if ( choiceinfo )
    {
        choiceinfo->m_choices = &m_choices;

        if ( !m_choices.IsOk() )
            return -1;

        choiceinfo->m_itemCount = m_choices.GetCount();

        if ( m_choices.GetCount() )
            choiceinfo->m_arrWxString = (wxString*)&m_choices.GetLabel(0);

    }

    if ( !m_choices.IsOk() )
        return -1;

    return m_choices.GetLabels().Index(m_value);
}

void wxCustomPropertyClass::SetAttribute ( int id, wxVariant& value )
{
#ifdef wxPG_COMPATIBILITY_1_0_0
    wxPropertyGrid* grid = GetGrid();
    if ( id == wxPG_CUSTOM_EDITOR )
    {
        if ( grid )
            grid->SetPropertyEditor( wxPGIdGen(this), (wxPGEditor*) value.GetVoidPtr() );
        else
            SetEditor( (wxPGEditor*) value.GetVoidPtr() );
    }
    else if ( id == wxPG_CUSTOM_IMAGE )
    {
        wxBitmap* bmp = (wxBitmap*) value.GetWxObjectPtr();
        if ( grid )
            grid->SetPropertyImage(wxPGIdGen(this),*bmp);
        else
            SetValueImage(*bmp);
    }
    else if ( id == wxPG_CUSTOM_CALLBACK )
    {
        m_callback = (wxPropertyGridCallback) value.GetVoidPtr();
    }
    else
#endif
    if ( id == wxPG_CUSTOM_PAINT_CALLBACK )
    {
        void* voidValue = value.GetVoidPtr();
        m_paintCallback = (wxPGPaintCallback) voidValue;
        if ( voidValue )
            m_flags |= wxPG_PROP_CUSTOMIMAGE;
        else if ( !GetValueImage() )
            m_flags &= ~(wxPG_PROP_CUSTOMIMAGE);
    }
    else
    if ( id == wxPG_CUSTOM_PRIVATE_CHILDREN )
    {
        if ( value.GetLong() )
            m_parentingType = -1;
        else
            m_parentingType = -2;
    }
}

// -----------------------------------------------------------------------

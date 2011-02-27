#include "Key_storage.h"
#include "Carman.h"
#include "Button.h"
#include "GUEventman.h"


// <editor-fold defaultstate="collapsed" desc="From file Initializer">

//--------------------------------------------------------------------------------------------------
template <>
bool St_loader<Key_id>::readFrag (ifstream &file)
{
	No_spaces_begin (file);
	char istr[1024] = "wrong value";
	file.getline (istr, 1024);
	Cut_end_spaces (istr);

	*_val = getKeyId (istr);

	return *_val != 0;
}
//--------------------------------------------------------------------------------------------------

class Key_storage::Initializer : public Sectionp
{
public:
	static GUEventman* evman;
	Key_storage ks;

protected:
	virtual bool beforeRead (ifstream &file)
	{
		ks.reset();
		return true;
	}
public:

	Initializer (const char* name)
	: Sectionp (name, '=')
	{
		ks.evman = evman;
		addParam (new St_loader<Key_id> ("up key", &ks.up));
		addParam (new St_loader<Key_id> ("down key", &ks.down));
		addParam (new St_loader<Key_id> ("left key", &ks.left));
		addParam (new St_loader<Key_id> ("right key", &ks.right));
	}

	virtual ~Initializer ()
	{
		deleteProps();
	}

	Key_storage* createKeyStorage() const
	{
		return ks.createCopy();
	}
};

GUEventman* Key_storage::Initializer::evman = 0;

class Key_storage::ParserHolder :public Control::ParserContainer
{
	Key_storage::Initializer parser;
public:
	ParserHolder (const char* name) :parser (name){}

	virtual Serializator* getParser()
	{
		return &parser;
	}
	virtual Control* createControl()
	{
		return parser.createKeyStorage();
	}
};

// </editor-fold>

void Key_storage::setControl (Car* c)
{
	Control::setControl (c);
	evman->registerKeyAction (new Arg_Method<void, void, Control> (this, &Control::upPress), EV_KEYDOWN, up);
	evman->registerKeyAction (new Arg_Method<void, void, Control> (this, &Control::upRelease), EV_KEYUP, up);
	evman->registerKeyAction (new Arg_Method<void, void, Control> (this, &Control::downPress), EV_KEYDOWN, down);
	evman->registerKeyAction (new Arg_Method<void, void, Control> (this, &Control::downRelease), EV_KEYUP, down);
	evman->registerKeyAction (new Arg_Method<void, void, Control> (this, &Control::leftPress), EV_KEYDOWN, left);
	evman->registerKeyAction (new Arg_Method<void, void, Control> (this, &Control::leftRelease), EV_KEYUP, left);
	evman->registerKeyAction (new Arg_Method<void, void, Control> (this, &Control::rightPress), EV_KEYDOWN, right);
	evman->registerKeyAction (new Arg_Method<void, void, Control> (this, &Control::rightRelease), EV_KEYUP, right);
}
//--------------------------------------------------------------------------------------------------
Key_storage* Key_storage::createCopy() const
{
	Key_storage* rez = new Key_storage;
	rez->down = down;
	rez->up = up;
	rez->left = left;
	rez->right = right;
	rez->evman = evman;
	return rez;
}
//--------------------------------------------------------------------------------------------------
void Key_storage::preInit (GUEventman* evman)
{
	Key_storage::Initializer::evman = evman;
	Control::registerParser (new Key_storage::ParserHolder ("keyboard"));
}
//--------------------------------------------------------------------------------------------------
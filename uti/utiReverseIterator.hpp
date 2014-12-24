#pragma once
#ifndef utiReverseIterator_h__
#define utiReverseIterator_h__

namespace uti
{
	template< typename Iterator >
	class ReverseIterator_tpl
	{
	public:

		ReverseIterator_tpl( const Iterator& it );

		ReverseIterator_tpl( const ReverseIterator_tpl< Iterator >& it );

		ReverseIterator_tpl< Iterator >& operator =( const ReverseIterator_tpl< Iterator >& it );
		ReverseIterator_tpl< Iterator >& operator =( const Iterator& it );

		typename Iterator::DataType& operator *( );

		bool Valid( void ) const;

		bool operator ==( const ReverseIterator_tpl< Iterator >& rhs ) const;
		bool operator !=( const ReverseIterator_tpl< Iterator >& rhs ) const;
		bool operator <=( const ReverseIterator_tpl< Iterator >& rhs ) const;
		bool operator <( const ReverseIterator_tpl< Iterator >& rhs ) const;
		bool operator >=( const ReverseIterator_tpl< Iterator >& rhs ) const;
		bool operator >( const ReverseIterator_tpl< Iterator >& rhs ) const;

		ReverseIterator_tpl< Iterator >& operator ++( void );
		ReverseIterator_tpl< Iterator > operator ++( int );


		ReverseIterator_tpl< Iterator >& operator --( void );
		ReverseIterator_tpl< Iterator > operator --( int );

	protected:
	private:

		Iterator m_It;
	};
}

#endif // utiReverseIterator_h__
